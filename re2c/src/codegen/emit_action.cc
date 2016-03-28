#include "src/util/c99_stdint.h"
#include <stddef.h>
#include <set>
#include <string>

#include "src/codegen/emit.h"
#include "src/codegen/input_api.h"
#include "src/codegen/output.h"
#include "src/conf/opt.h"
#include "src/globals.h"
#include "src/ir/adfa/action.h"
#include "src/ir/adfa/adfa.h"
#include "src/ir/regexp/regexp.h"
#include "src/ir/skeleton/skeleton.h"
#include "src/parse/code.h"
#include "src/parse/loc.h"

namespace re2c
{

class label_t;

static void need               (OutputFile & o, uint32_t ind, bool & readCh, size_t n, bool bSetMarker);
static void emit_match         (OutputFile & o, uint32_t ind, bool & readCh, const State * const s);
static void emit_initial       (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const Initial & init, const std::set<label_t> & used_labels);
static void emit_save          (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, uint32_t save, bool save_yyaccept);
static void emit_accept_binary (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const accept_t & accept, size_t l, size_t r);
static void emit_accept        (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const accept_t & accept);
static void emit_rule(OutputFile &o, uint32_t ind, const State *const s,
	size_t rule, const std::string &condName,
	const Skeleton *skeleton, bool base_ctxmarker);
static void genYYFill          (OutputFile & o, size_t need);
static void genSetCondition    (OutputFile & o, uint32_t ind, const std::string & newcond);
static void genSetState        (OutputFile & o, uint32_t ind, uint32_t fillIndex);

void emit_action
	( const Action & action
	, OutputFile & o
	, uint32_t ind
	, bool & readCh
	, const State * const s
	, const std::string & condName
	, const Skeleton * skeleton
	, const std::set<label_t> & used_labels
	, bool save_yyaccept
	, bool base_ctxmarker
	)
{
	switch (action.type)
	{
		case Action::MATCH:
			emit_match (o, ind, readCh, s);
			break;
		case Action::INITIAL:
			emit_initial (o, ind, readCh, s, * action.info.initial, used_labels);
			break;
		case Action::SAVE:
			emit_save (o, ind, readCh, s, action.info.save, save_yyaccept);
			break;
		case Action::MOVE:
			break;
		case Action::ACCEPT:
			emit_accept (o, ind, readCh, s, * action.info.accepts);
			break;
		case Action::RULE:
			emit_rule (o, ind, s, action.info.rule, condName, skeleton, base_ctxmarker);
			break;
	}
	if (opts->target != opt_t::DOT && !s->ctxs.empty()) {
		if (base_ctxmarker) {
			o.wstring(opts->input_api.stmt_dist(ind, s->ctxs, skeleton->contexts));
		} else {
			o.wstring(opts->input_api.stmt_backupctx(ind));
		}
	}
}

void emit_match (OutputFile & o, uint32_t ind, bool & readCh, const State * const s)
{
	if (opts->target == opt_t::DOT)
	{
		return;
	}

	const bool read_ahead = s
		&& s->next
		&& s->next->action.type != Action::RULE;
	if (s->fill != 0)
	{
		o.wstring(opts->input_api.stmt_skip (ind));
	}
	else if (!read_ahead)
	{
		/* do not read next char if match */
		o.wstring(opts->input_api.stmt_skip (ind));
		readCh = true;
	}
	else
	{
		o.wstring(opts->input_api.stmt_skip_peek (ind));
		readCh = false;
	}

	if (s->fill != 0)
	{
		need(o, ind, readCh, s->fill, false);
	}
}

void emit_initial (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const Initial & initial, const std::set<label_t> & used_labels)
{
	if (opts->target == opt_t::DOT)
	{
		return;
	}

	if (used_labels.count(s->label))
	{
		if (s->fill != 0)
		{
			o.wstring(opts->input_api.stmt_skip (ind));
		}
		else
		{
			o.wstring(opts->input_api.stmt_skip_peek (ind));
		}
	}

	if (used_labels.count(initial.label))
	{
		o.wstring(opts->labelPrefix).wlabel(initial.label).ws(":\n");
	}

	if (opts->dFlag)
	{
		o.wind(ind).wstring(opts->yydebug).ws("(").wlabel(initial.label).ws(", *").wstring(opts->yycursor).ws(");\n");
	}

	if (s->fill != 0)
	{
		need(o, ind, readCh, s->fill, initial.setMarker);
	}
	else
	{
		if (initial.setMarker)
		{
			o.wstring(opts->input_api.stmt_backup (ind));
		}
		readCh = false;
	}
}

void emit_save (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, uint32_t save, bool save_yyaccept)
{
	if (opts->target == opt_t::DOT)
	{
		return;
	}

	if (save_yyaccept)
	{
		o.wind(ind).wstring(opts->yyaccept).ws(" = ").wu32(save).ws(";\n");
	}

	if (s->fill != 0)
	{
		o.wstring(opts->input_api.stmt_skip_backup (ind));
		need(o, ind, readCh, s->fill, false);
	}
	else
	{
		o.wstring(opts->input_api.stmt_skip_backup_peek (ind));
		readCh = false;
	}
}

void emit_accept_binary (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const accept_t & accepts, size_t l, size_t r)
{
	if (l < r)
	{
		const size_t m = (l + r) >> 1;
		o.wind(ind).ws("if (").wstring(opts->yyaccept).ws(r == l+1 ? " == " : " <= ").wu64(m).ws(") {\n");
		emit_accept_binary (o, ++ind, readCh, s, accepts, l, m);
		o.wind(--ind).ws("} else {\n");
		emit_accept_binary (o, ++ind, readCh, s, accepts, m + 1, r);
		o.wind(--ind).ws("}\n");
	}
	else
	{
		genGoTo(o, ind, s, accepts[l], readCh);
	}
}

void emit_accept (OutputFile & o, uint32_t ind, bool & readCh, const State * const s, const accept_t & accepts)
{
	const size_t accepts_size = accepts.size ();
	if (accepts_size > 0)
	{
		if (opts->target != opt_t::DOT)
		{
			o.wstring(opts->input_api.stmt_restore (ind));
		}

		if (readCh) // shouldn't be necessary, but might become at some point
		{
			o.wstring(opts->input_api.stmt_peek (ind));
			readCh = false;
		}

		if (accepts_size > 1)
		{
			if (opts->gFlag && accepts_size >= opts->cGotoThreshold)
			{
				o.wind(ind++).ws("{\n");
				o.wind(ind++).ws("static void *").wstring(opts->yytarget).ws("[").wu64(accepts_size).ws("] = {\n");
				for (uint32_t i = 0; i < accepts_size; ++i)
				{
					o.wind(ind).ws("&&").wstring(opts->labelPrefix).wlabel(accepts[i]->label).ws(",\n");
				}
				o.wind(--ind).ws("};\n");
				o.wind(ind).ws("goto *").wstring(opts->yytarget).ws("[").wstring(opts->yyaccept).ws("];\n");
				o.wind(--ind).ws("}\n");
			}
			else if (opts->sFlag || (accepts_size == 2 && opts->target != opt_t::DOT))
			{
				emit_accept_binary (o, ind, readCh, s, accepts, 0, accepts_size - 1);
			}
			else if (opts->target == opt_t::DOT)
			{
				for (uint32_t i = 0; i < accepts_size; ++i)
				{
					o.wlabel(s->label).ws(" -> ").wlabel(accepts[i]->label);
					o.ws(" [label=\"yyaccept=").wu32(i).ws("\"]\n");
				}
			}
			else
			{
				o.wind(ind).ws("switch (").wstring(opts->yyaccept).ws(") {\n");
				for (uint32_t i = 0; i < accepts_size - 1; ++i)
				{
					o.wind(ind).ws("case ").wu32(i).ws(": \t");
					genGoTo(o, 0, s, accepts[i], readCh);
				}
				o.wind(ind).ws("default:\t");
				genGoTo(o, 0, s, accepts[accepts_size - 1], readCh);
				o.wind(ind).ws("}\n");
			}
		}
		else
		{
			// no need to write if statement here since there is only case 0.
			genGoTo(o, ind, s, accepts[0], readCh);
		}
	}
}

static void rreplace_substr(std::string &s,
	const std::string &s1, const std::string &s2)
{
	if (!s1.empty()) {
		std::string::size_type pos;
		while ((pos = s.find(s1)) != std::string::npos) {
			s.replace(pos, s1.length(), s2);
		}
	}
}

static void subst_contexts(
	std::string &action,
	const Rule &rule,
	const std::vector<CtxVar> &contexts)
{
	for (size_t i = 0; i < rule.ctxvar.size(); ++i) {
		const CtxVar &ctx = contexts[rule.ctxvar[i]];
		rreplace_substr(action, "@" + *ctx.name,
			opts->input_api.expr_ctx(ctx.fullname));
	}

	for (size_t i = 0; i < rule.ctxfix.size(); ++i) {
		const CtxFix &ctx = rule.ctxfix[i];
		const std::string basename = (ctx.base == CtxFix::RIGHTMOST)
			? opts->input_api.expr_dist()
			: contexts[ctx.base].fullname;
		std::ostringstream offs;
		offs << "(" << basename << " - " << ctx.dist << ")";
		const std::string ctx_value = opts->input_api.expr_ctx(offs.str());
		rreplace_substr(action, "@" + *ctx.name, ctx_value);
	}
}

void emit_rule(
	OutputFile &o,
	uint32_t ind,
	const State *const s,
	size_t rule_idx,
	const std::string &condName,
	const Skeleton *skeleton,
	bool base_ctxmarker)
{
	const Rule &rule = skeleton->rules[rule_idx];
	const RuleInfo *info = rule.info;

	if (opts->target == opt_t::DOT) {
		o.wlabel(s->label);
		const Code *code = info->code;
		if (code) {
			o.ws(" [label=\"").wstring(code->loc.filename)
				.ws(":").wu32(code->loc.line).ws("\"]");
		}
		o.ws("\n");
		return;
	}

	const Trail &trail = rule.trail;
	switch (trail.type) {
		case Trail::NONE:
			break;
		case Trail::VAR:
			if (base_ctxmarker) {
				const std::string name = skeleton->contexts[trail.pld.var].fullname;
				o.wstring(opts->input_api.stmt_restorectx_var_base(ind, name));
			} else {
				o.wstring(opts->input_api.stmt_restorectx_var(ind));
			}
			break;
		case Trail::FIX:
			o.wstring(opts->input_api.stmt_restorectx_fix(ind, trail.pld.fix));
			break;
	}

	if (opts->target == opt_t::SKELETON) {
		emit_action(*skeleton, o, ind, rule_idx);
	} else {
		const std::string &newcond = info->newcond;
		if (!newcond.empty() && condName != newcond) {
			genSetCondition(o, ind, newcond);
		}
		const Code *code = info->code;
		if (code) {
			if (!yySetupRule.empty()) {
				o.wind(ind).wstring(yySetupRule).ws("\n");
			}
			std::string action = code->text;
			subst_contexts(action, rule, skeleton->contexts);
			o.wline_info(code->loc.line, code->loc.filename.c_str())
				.wind(ind).wstring(action).ws("\n")
				.wdelay_line_info();
		} else if (!newcond.empty()) {
			const std::string action = replaceParam(opts->condGoto,
				opts->condGotoParam, opts->condPrefix + newcond);
			o.wind(ind).wstring(action).ws("\n");
		}
	}
}

void need (OutputFile & o, uint32_t ind, bool & readCh, size_t n, bool bSetMarker)
{
	if (opts->target == opt_t::DOT)
	{
		return;
	}

	uint32_t fillIndex = last_fill_index;

	if (opts->fFlag)
	{
		last_fill_index++;
		genSetState (o, ind, fillIndex);
	}

	if (opts->fill_use && n > 0)
	{
		o.wind(ind);
		if (n == 1)
		{
			if (opts->fill_check)
			{
				o.ws("if (").wstring(opts->input_api.expr_lessthan_one ()).ws(") ");
			}
			genYYFill(o, n);
		}
		else
		{
			if (opts->fill_check)
			{
				o.ws("if (").wstring(opts->input_api.expr_lessthan (n)).ws(") ");
			}
			genYYFill(o, n);
		}
	}

	if (opts->fFlag)
	{
		o.wstring(opts->yyfilllabel).wu32(fillIndex).ws(":\n");
	}

	if (n > 0)
	{
		if (bSetMarker)
		{
			o.wstring(opts->input_api.stmt_backup_peek (ind));
		}
		else
		{
			o.wstring(opts->input_api.stmt_peek (ind));
		}
		readCh = false;
	}
}

void genYYFill (OutputFile & o, size_t need)
{
	o.wstring(replaceParam (opts->fill, opts->fill_arg, need));
	if (!opts->fill_naked)
	{
		if (opts->fill_arg_use)
		{
			o.ws("(").wu64(need).ws(")");
		}
		o.ws(";");
	}
	o.ws("\n");
}

void genSetCondition(OutputFile & o, uint32_t ind, const std::string& newcond)
{
	o.wind(ind).wstring(replaceParam (opts->cond_set, opts->cond_set_arg, opts->condEnumPrefix + newcond));
	if (!opts->cond_set_naked)
	{
		o.ws("(").wstring(opts->condEnumPrefix).wstring(newcond).ws(");");
	}
	o.ws("\n");
}

void genSetState(OutputFile & o, uint32_t ind, uint32_t fillIndex)
{
	o.wind(ind).wstring(replaceParam (opts->state_set, opts->state_set_arg, fillIndex));
	if (!opts->state_set_naked)
	{
		o.ws("(").wu32(fillIndex).ws(");");
	}
	o.ws("\n");
}

} // namespace re2c
