
/* Declarations */
%code requires
{
#include <Arduino.h>
#include <math.h>
class FlowController;
void yyerror(FlowController *flow, bool *hasResult, float *result, const char *msg);
}

%union { float val; }
/* %destructor { type1_free ($$); } <val> */

/* BISON Declarations */
%define api.pure full
%define api.push-pull push

%parse-param {FlowController *flow} {bool *hasResult} {float *result}

/* %error-verbose */

%token <val> NUM
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

%type <val> input exp

/* Grammar follows */
%%

input
        : exp '\n' { *hasResult = true; *result = $1; YYACCEPT; }
        | '\n' { *hasResult = false; YYACCEPT; }
        ;

exp
        : NUM                           { $$ = $1;         }
        | exp '+' exp                   { $$ = $1 + $3;    }
        | exp '+' newlines exp          { $$ = $1 + $4;    }
        | exp '-' exp                   { $$ = $1 - $3;    }
        | exp '-' newlines exp          { $$ = $1 - $4;    }
        | exp '*' exp                   { $$ = $1 * $3;    }
        | exp '*' newlines exp          { $$ = $1 * $4;    }
        | exp '/' exp                   { $$ = $1 / $3;    }
        | exp '/' newlines exp          { $$ = $1 / $4;    }
        | '-' exp %prec NEG             { $$ = -$2;        }
        | exp '^' exp                   { $$ = pow ($1, $3); }
        | '(' exp ')'                   { $$ = $2;         }
        | '(' exp newlines ')'          { $$ = $2;         }
        | '(' newlines exp ')'          { $$ = $3;         }
        | '(' newlines exp newlines ')' { $$ = $3;         }
        ;

newlines
        : '\n'
        | newlines '\n'
        ;

%%
