/* Generated by re2c */
// This example shows that fallback tags should participate
// in tag optimization: there is a chance that they will be
// merged with other tags.

yyt1
yyt2


{
	YYCTYPE yych;
	if ((YYLIMIT - YYCURSOR) < 3) YYFILL(3);
	yych = *YYCURSOR;
	switch (yych) {
	case 'a':
		yyt1 = YYCURSOR;
		goto yy5;
	default:	goto yy3;
	}
yy2:
	YYCURSOR = yyt1;
	{}
yy3:
	++YYCURSOR;
yy4:
	{}
yy5:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'b':
		yyt2 = YYCURSOR;
		goto yy6;
	default:	goto yy4;
	}
yy6:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'c':	goto yy7;
	default:	goto yy2;
	}
yy7:
	YYMARKER = ++YYCURSOR;
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	yyt1 = yyt2;
	switch (yych) {
	case 'a':	goto yy9;
	default:	goto yy8;
	}
yy8:
	p = yyt1;
	{ p }
yy9:
	++YYCURSOR;
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch (yych) {
	case 'b':
		yyt2 = YYCURSOR;
		goto yy11;
	default:	goto yy10;
	}
yy10:
	YYCURSOR = YYMARKER;
	goto yy8;
yy11:
	++YYCURSOR;
	if (YYLIMIT <= YYCURSOR) YYFILL(1);
	yych = *YYCURSOR;
	switch (yych) {
	case 'c':	goto yy7;
	default:	goto yy10;
	}
}

re2c: warning: line 9: rule matches empty string [-Wmatch-empty-string]
