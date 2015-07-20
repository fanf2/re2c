#include <string.h>

#include "src/codegen/bitmap.h"
#include "src/codegen/scc.h"
#include "src/ir/dfa/dfa.h"
#include "src/ir/regexp/regexp_rule.h"
#include "src/util/allocate.h"

namespace re2c {

void DFA::findSCCs()
{
	SCC scc(nStates);
	State *s;

	for (s = head; s; s = s->next)
	{
		s->depth = 0;
		s->link = NULL;
	}

	for (s = head; s; s = s->next)
	{
		if (!s->depth)
		{
			scc.traverse(s);
		}
	}

	calcDepth(head);
}

void DFA::split(State *s)
{
	State *move = new State;
	addState(&s->next, move);
	move->action.set_move ();
	move->link = s->link;
	move->rule = s->rule;
	move->go = s->go;
	s->rule = NULL;
	s->go.nSpans = 1;
	s->go.span = allocate<Span> (1);
	s->go.span[0].ub = ubChar;
	s->go.span[0].to = move;
}

static uint32_t merge(Span *x0, State *fg, State *bg)
{
	Span *x = x0, *f = fg->go.span, *b = bg->go.span;
	uint32_t nf = fg->go.nSpans, nb = bg->go.nSpans;
	State *prev = NULL, *to;
	// NB: we assume both spans are for same range

	for (;;)
	{
		if (f->ub == b->ub)
		{
			to = f->to == b->to ? bg : f->to;

			if (to == prev)
			{
				--x;
			}
			else
			{
				x->to = prev = to;
			}

			x->ub = f->ub;
			++x;
			++f;
			--nf;
			++b;
			--nb;

			if (nf == 0 && nb == 0)
			{
				return x - x0;
			}
		}

		while (f->ub < b->ub)
		{
			to = f->to == b->to ? bg : f->to;

			if (to == prev)
			{
				--x;
			}
			else
			{
				x->to = prev = to;
			}

			x->ub = f->ub;
			++x;
			++f;
			--nf;
		}

		while (b->ub < f->ub)
		{
			to = b->to == f->to ? bg : f->to;

			if (to == prev)
			{
				--x;
			}
			else
			{
				x->to = prev = to;
			}

			x->ub = b->ub;
			++x;
			++b;
			--nb;
		}
	}
}

void DFA::findBaseState()
{
	Span *span = allocate<Span> (ubChar - lbChar);

	for (State *s = head; s; s = s->next)
	{
		if (!s->link)
		{
			for (uint32_t i = 0; i < s->go.nSpans; ++i)
			{
				State *to = s->go.span[i].to;

				if (to->isBase)
				{
					to = to->go.span[0].to;
					uint32_t nSpans = merge(span, s, to);

					if (nSpans < s->go.nSpans)
					{
						operator delete (s->go.span);
						s->go.nSpans = nSpans;
						s->go.span = allocate<Span> (nSpans);
						memcpy(s->go.span, span, nSpans*sizeof(Span));
					}

					break;
				}
			}
		}
	}

	operator delete (span);
}

void DFA::prepare(OutputFile & o, uint32_t & max_fill)
{
	bUsedYYBitmap = false;

	findSCCs();
	head->link = head;

	for (State * s = head; s; s = s->next)
	{
		s->depth = maxDist(s);
		if (max_fill < s->depth)
		{
			max_fill = s->depth;
		}
	}

	// create rule states
	std::map<rule_rank_t, State *> rules;
	for (State * s = head; s; s = s->next)
	{
		if (s->rule)
		{
			if (rules.find (s->rule->rank) == rules.end ())
			{
				State *n = new State;
				n->action.set_rule (s->rule);
				rules[s->rule->rank] = n;
				addState(&s->next, n);
			}
			for (uint32_t i = 0; i < s->go.nSpans; ++i)
			{
				if (!s->go.span[i].to)
				{
					s->go.span[i].to = rules[s->rule->rank];
				}
			}
		}
	}

	// create default state (if needed)
	State * default_state = NULL;
	for (State * s = head; s; s = s->next)
	{
		for (uint32_t i = 0; i < s->go.nSpans; ++i)
		{
			if (!s->go.span[i].to)
			{
				if (!default_state)
				{
					default_state = new State;
					addState(&s->next, default_state);
				}
				s->go.span[i].to = default_state;
			}
		}
	}

	// find backup states and create accept state (if needed)
	if (default_state)
	{
		for (State * s = head; s; s = s->next)
		{
			if (s->rule)
			{
				for (uint32_t i = 0; i < s->go.nSpans; ++i)
				{
					if (!s->go.span[i].to->rule && s->go.span[i].to->action.type != Action::RULE)
					{
						const uint32_t accept = accepts.find_or_add (rules[s->rule->rank]);
						s->action.set_save (accept);
					}
				}
			}
		}
		if (accepts.size () > 1)
		{
			o.set_used_yyaccept ();
		}
		default_state->action.set_accept (&accepts);
	}

	// split ``base'' states into two parts
	for (State * s = head; s; s = s->next)
	{
		s->isBase = false;

		if (s->link)
		{
			for (uint32_t i = 0; i < s->go.nSpans; ++i)
			{
				if (s->go.span[i].to == s)
				{
					s->isBase = true;
					split(s);

					if (bFlag)
					{
						BitMap::find(&s->next->go, s);
					}

					s = s->next;
					break;
				}
			}
		}
	}

	// find ``base'' state, if possible
	findBaseState();

	for (State * s = head; s; s = s->next)
	{
		s->go.init (s);
	}
}

} // namespace re2c