
/* Declarations */
%code requires
{
#include <Arduino.h>
#include <math.h>
#include "../Ast/Ast.h"
#include "../Ast/Funcs.h"
class FlowController;
void yyerror(FlowController *flow, bool *hasResult, Expr **result, const char *msg);
}

%union
{
    float number;
    uint32_t ident;
    Expr *expr;
}
/* %destructor { type1_free ($$); } <val> */

/* BISON Declarations */
%define api.pure full
%define api.push-pull push

%parse-param {FlowController *flow} {bool *hasResult} {Expr **result}

/* %error-verbose */

%token <number> NUMBER
%token <ident> IDENTIFIER

%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

%type <expr> input expr

/* Grammar follows */
%%

input
    : expr '\n' { *hasResult = true; *result = $1; YYACCEPT; }
    | '\n' { *hasResult = false; YYACCEPT; }
    ;

expr
    : NUMBER                            { $$ = new NumberExpr($1);               }
    | IDENTIFIER                        { $$ = new NumberExpr(0);                }
    | expr '+' expr                     { $$ = new FuncallExpr(addFunc, $1, $3);}
    | expr '+' newlines expr            { $$ = new FuncallExpr(addFunc, $1, $4);}
    | expr '-' expr                     { $$ = new FuncallExpr(subFunc, $1, $3);}
    | expr '-' newlines expr            { $$ = new FuncallExpr(subFunc, $1, $4);}
    | expr '*' expr                     { $$ = new FuncallExpr(mulFunc, $1, $3);}
    | expr '*' newlines expr            { $$ = new FuncallExpr(mulFunc, $1, $4);}
    | expr '/' expr                     { $$ = new FuncallExpr(divFunc, $1, $3);}
    | expr '/' newlines expr            { $$ = new FuncallExpr(divFunc, $1, $4);}
    | '-' expr %prec NEG                { $$ = new FuncallExpr(negFunc, $2);}
    | expr '^' expr                     { $$ = new FuncallExpr(powFunc, $1, $3);}
    | '(' expr ')'                      { $$ = $2;         }
    | '(' expr newlines ')'             { $$ = $2;         }
    | '(' newlines expr ')'             { $$ = $3;         }
    | '(' newlines expr newlines ')'    { $$ = $3;         }
    ;

newlines
    : '\n'
    | newlines '\n'
    ;

%%
