re2c: warning: line 14: control flow is undefined for strings that match '[\x0-\x6\xE-\x21\x23-\x26\x28-\x5B\x5D-\xFF]', use default rule '*' [-Wundefined-control-flow]
/* Generated by re2c */

digraph re2c {

1 -> 2 [label="[0x00-0x06][0x0E-!][#-&][(-[][0x5D-0xFF]"]
1 -> 19 [label="[0x07]"]
1 -> 13 [label="[0x08]"]
1 -> 9 [label="[0x09]"]
1 -> 7 [label="[0x0A]"]
1 -> 11 [label="[0x0B]"]
1 -> 17 [label="[0x0C]"]
1 -> 15 [label="[0x0D]"]
1 -> 5 [label="[\"]"]
1 -> 3 [label="[']"]
1 -> 21 [label="[\\]"]
3 -> 4
4 [label="bug47_dot_escapes.D.re:3"]
5 -> 6
6 [label="bug47_dot_escapes.D.re:4"]
7 -> 8
8 [label="bug47_dot_escapes.D.re:5"]
9 -> 10
10 [label="bug47_dot_escapes.D.re:6"]
11 -> 12
12 [label="bug47_dot_escapes.D.re:7"]
13 -> 14
14 [label="bug47_dot_escapes.D.re:8"]
15 -> 16
16 [label="bug47_dot_escapes.D.re:9"]
17 -> 18
18 [label="bug47_dot_escapes.D.re:10"]
19 -> 20
20 [label="bug47_dot_escapes.D.re:11"]
21 -> 22
22 [label="bug47_dot_escapes.D.re:12"]
}
