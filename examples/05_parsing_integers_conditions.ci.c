/* Generated by re2c */
#include <limits.h>
#include <stdio.h>

template<int base>
static bool adddgt(unsigned long &u, unsigned int d)
{
    if (u > (ULONG_MAX - d) / base) {
        return false;
    }
    u = u * base + d;
    return true;
}

enum YYCONDTYPE {
	yycinit,
	yycbin,
	yycdec,
	yychex,
	yycoct,
};


static bool lex(const char *s, unsigned long &u)
{
    const char *YYMARKER;
    const char *YYCTXMARKER;
    int c = yycinit;
    u = 0;
    
{
	char yych;
	switch (c) {
	case yycinit:
		goto yyc_init;
	case yycbin:
		goto yyc_bin;
	case yycdec:
		goto yyc_dec;
	case yychex:
		goto yyc_hex;
	case yycoct:
		goto yyc_oct;
	}
/* *********************************** */
yyc_init:
	yych = *s;
	switch (yych) {
	case '0':	goto yy4;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy6;
	default:	goto yy2;
	}
yy2:
	++s;
	{ return false; }
yy4:
	yych = *(YYMARKER = ++s);
	switch (yych) {
	case 'B':
	case 'b':	goto yy8;
	case 'X':
	case 'x':	goto yy10;
	default:	goto yy5;
	}
yy5:
	c = yycoct;
	goto yyc_oct;
yy6:
	++s;
	s -= 1;
	c = yycdec;
	goto yyc_dec;
yy8:
	yych = *++s;
	switch (yych) {
	case '0':
	case '1':	goto yy11;
	default:	goto yy9;
	}
yy9:
	s = YYMARKER;
	goto yy5;
yy10:
	yych = *++s;
	switch (yych) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy13;
	default:	goto yy9;
	}
yy11:
	++s;
	s -= 1;
	c = yycbin;
	goto yyc_bin;
yy13:
	++s;
	s -= 1;
	c = yychex;
	goto yyc_hex;
/* *********************************** */
yyc_bin:
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy17;
	case '0':
	case '1':	goto yy21;
	default:	goto yy19;
	}
yy17:
	++s;
	{ return true; }
yy19:
	++s;
	{ return false; }
yy21:
	++s;
	{ if (!adddgt<2>(u, s[-1] - '0')) return false; goto yyc_bin; }
/* *********************************** */
yyc_dec:
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy25;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy29;
	default:	goto yy27;
	}
yy25:
	++s;
	{ return true; }
yy27:
	++s;
	{ return false; }
yy29:
	++s;
	{ if (!adddgt<10>(u, s[-1] - '0')) return false; goto yyc_dec; }
/* *********************************** */
yyc_hex:
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy33;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':	goto yy37;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':	goto yy39;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':	goto yy41;
	default:	goto yy35;
	}
yy33:
	++s;
	{ return true; }
yy35:
	++s;
	{ return false; }
yy37:
	++s;
	{ if (!adddgt<16>(u, s[-1] - '0'))      return false; goto yyc_hex; }
yy39:
	++s;
	{ if (!adddgt<16>(u, s[-1] - 'A' + 10)) return false; goto yyc_hex; }
yy41:
	++s;
	{ if (!adddgt<16>(u, s[-1] - 'a' + 10)) return false; goto yyc_hex; }
/* *********************************** */
yyc_oct:
	yych = *s;
	switch (yych) {
	case 0x00:	goto yy45;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':	goto yy49;
	default:	goto yy47;
	}
yy45:
	++s;
	{ return true; }
yy47:
	++s;
	{ return false; }
yy49:
	++s;
	{ if (!adddgt<8>(u, s[-1] - '0')) return false; goto yyc_oct; }
}

}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        unsigned long u;
        if (lex(argv[i], u)) {
            printf("%lu\n", u);
        } else {
            printf("error\n");
        }
    }
    return 0;
}
05_parsing_integers_conditions.ci.re:35:34: warning: rule in condition 'init' matches empty string [-Wmatch-empty-string]
