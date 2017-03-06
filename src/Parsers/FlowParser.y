
/* Declarations */
%code top
{

}
%code requires
{
#include <Arduino.h>
#include "../DOM/Node.h"
class FlowController;
void yyerror(FlowController *flow, bool *hasResult, Node **result, const char *msg);
}

%union
{
    float number;
    uint32_t ident;
    Node *node;
}

%destructor { delete ($$); } <node>

/* BISON Declarations */
%define api.pure full
%define api.push-pull push

%parse-param {FlowController *flow} {bool *hasResult} {Node **result}

/* %error-verbose */

%token <number> NUMBER
%token <ident> IDENTIFIER

%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

%type <node> input expr

/* Grammar follows */
%%

input
    : expr '\n' { $$ = $1; *hasResult = true; *result = $1; YYACCEPT; }
    | '\n' { $$ = 0; *hasResult = false; YYACCEPT; }
    ;

expr
    : NUMBER                            { $$ = Node::createNumberLiteral($1);              }
    | IDENTIFIER                        { $$ = Node::createNumberLiteral(0);               }
    | expr '+' expr                     { $$ = Node::createBinaryOperator(BO_Add, $1, $3); }
    | expr '+' newlines expr            { $$ = Node::createBinaryOperator(BO_Add, $1, $4); }
    | expr '-' expr                     { $$ = Node::createBinaryOperator(BO_Sub, $1, $3); }
    | expr '-' newlines expr            { $$ = Node::createBinaryOperator(BO_Sub, $1, $4); }
    | expr '*' expr                     { $$ = Node::createBinaryOperator(BO_Mul, $1, $3); }
    | expr '*' newlines expr            { $$ = Node::createBinaryOperator(BO_Mul, $1, $4); }
    | expr '/' expr                     { $$ = Node::createBinaryOperator(BO_Div, $1, $3); }
    | expr '/' newlines expr            { $$ = Node::createBinaryOperator(BO_Div, $1, $4); }
    | '-' expr %prec NEG                { $$ = Node::createUnaryOperator(UO_Negate, $2);   }
    | expr '^' expr                     { $$ = Node::createBinaryOperator(BO_Pow, $1, $3); }
    | '(' expr ')'                      { $$ = $2; }
    | '(' expr newlines ')'             { $$ = $2; }
    | '(' newlines expr ')'             { $$ = $3; }
    | '(' newlines expr newlines ')'    { $$ = $3; }
    ;

newlines
    : '\n'
    | newlines '\n'
    ;

%%
