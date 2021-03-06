/*!rules:re2c
  <c1> [a] :=> c2
  <c2> [b] :=> c1
  <*> * { x }
*/

// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

// re2c:define:YYSETCONDITION@cond = @@;
// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = @@;
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:define:YYSETCONDITION@cond = @@;
// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = @@;
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

// re2c:define:YYSETCONDITION@cond = @@;
// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = @@;
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:define:YYSETCONDITION@cond = @@;
// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = @@;
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

// re2c:define:YYSETCONDITION@cond = #;
// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = #;
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:define:YYSETCONDITION@cond = #;
// re2c:placeholder = #;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = #;
  re2c:placeholder = #;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

// re2c:define:YYSETCONDITION@cond = #;
// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = @@;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = #;
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = @@;";
*/

// re2c:define:YYSETCONDITION@cond = #;
// re2c:placeholder = @@;
// re2c:define:YYSETCONDITION = "cond = #;";
/*!use:re2c
  re2c:define:YYSETCONDITION@cond = #;
  re2c:placeholder = @@;
  re2c:define:YYSETCONDITION = "cond = #;";
*/

