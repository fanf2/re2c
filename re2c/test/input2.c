re2c: warning: line 5: control flow is undefined for strings that match 
	'[\x0-\x60\x62-\xFF]'
	'\x61 [\x0-\x60\x63-\xFF]'
	'\x61 \x61 [\x0-\x60\x63-\xFF]'
	'\x61 \x62 [\x0-\x61\x63-\xFF]'
	'\x61 \x61 \x62 [\x0-\x61\x63-\xFF]'
, use default rule '*' [-Wundefined-control-flow]
/* Generated by re2c */
#line 1 "input2.re"

#line 5 "<stdout>"
{
	YYCTYPE yych;
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch (yych) {
	case 'a':	goto yy3;
	default:	goto yy2;
	}
yy2:
yy3:
	++YYCURSOR;
	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
	yych = *YYCURSOR;
	switch (yych) {
	case 'a':	goto yy3;
	case 'b':	goto yy5;
	default:	goto yy2;
	}
yy5:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'b':	goto yy6;
	default:	goto yy2;
	}
yy6:
	++YYCURSOR;
#line 3 "input2.re"
	{ return 1; }
#line 34 "<stdout>"
}
#line 5 "input2.re"

