re2c: warning: line 3: control flow is undefined for strings that match '[\x0-\xFFFF\x10001-\x10FFFF]', use default rule '*' [-Wundefined-control-flow]
/* Generated by re2c */
#line 1 "utf32_0x10000.u.re"

#line 5 "<stdout>"
{
	YYCTYPE yych;
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	if (yych == 0x00010000) goto yy3;
yy3:
	++YYCURSOR;
#line 2 "utf32_0x10000.u.re"
	{}
#line 15 "<stdout>"
}
#line 3 "utf32_0x10000.u.re"

