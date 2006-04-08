/* Generated by re2c */
#line 1 "bug1454253b.s.re"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RET(n) return n

size_t scan(const char *s, int l, char *r)
{
	const char *p = s;
	const char *q;
	#define YYCTYPE         char
	#define YYCURSOR        p
	#define YYLIMIT         (s+l)
	#define YYMARKER        q
	#define YYFILL(n)

	while(1)
	{

#line 23 "<stdout>"
{
	YYCTYPE yych;

	if((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	if(yych != '?') goto yy4;
	yych = *++YYCURSOR;
	if(yych <= '9') {
		if(yych == '!') goto yy9;
		if(yych >= '0') goto yy6;
	} else {
		if(yych <= 'Z') {
			if(yych >= 'A') goto yy6;
		} else {
			if(yych <= '`') goto yy3;
			if(yych <= 'z') goto yy6;
		}
	}
yy3:
yy4:
	++YYCURSOR;
#line 33 "bug1454253b.s.re"
	{
		*r++ = '0';
		*r++ = '\0';
		return p - s;
	}
#line 51 "<stdout>"
yy6:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if(yych <= '@') {
		if(yych <= '/') goto yy8;
		if(yych <= '9') goto yy6;
	} else {
		if(yych <= 'Z') goto yy6;
		if(yych <= '`') goto yy8;
		if(yych <= 'z') goto yy6;
	}
yy8:
#line 28 "bug1454253b.s.re"
	{
		*r++ = '2';
		continue;
	}
#line 70 "<stdout>"
yy9:
	++YYCURSOR;
	if(YYLIMIT == YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if(yych <= '@') {
		if(yych <= '/') goto yy11;
		if(yych <= '9') goto yy9;
	} else {
		if(yych <= 'Z') goto yy9;
		if(yych <= '`') goto yy11;
		if(yych <= 'z') goto yy9;
	}
yy11:
#line 23 "bug1454253b.s.re"
	{
		*r++ = '1';
		continue;
	}
#line 89 "<stdout>"
}
#line 38 "bug1454253b.s.re"

	}
}

void _do_scan(const char * str, size_t str_len, size_t scn_len, const char * exp, size_t exp_len)
{
	char res[32], sp[32];
	size_t res_len, inp_len;

	inp_len = scan(str, str_len, res);
	res_len = strlen(res);
	
	memset(sp, ' ', sizeof(sp));
	sp[15 - str_len] = '\0';

	printf("[%s](%2d/%2d)%s: %8s(%d) %s %8s(%d)\n", str, inp_len, scn_len, sp, 
			exp, exp_len, 
			inp_len != scn_len || exp_len != res_len || strncmp(exp, res, exp_len) ? "!=" : "==", 
			res, res_len);
}

#define do_scan(str, scn_len, exp) _do_scan(str, sizeof(str) - 1, scn_len, exp, sizeof(exp) - 1)

main()
{
	do_scan("#",            1, "0");
	do_scan("?!?a+",        5, "120");
	do_scan("?1?2!1?2??",   5, "220");
	do_scan("?1?2?!1?2??", 11, "22120");
	do_scan("?1?2?!1?2?!", 12, "221210");
	do_scan("?1?123?45??", 11, "2220");
	do_scan("?1?123?45?!", 12, "22210");
}
