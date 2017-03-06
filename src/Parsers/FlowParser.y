
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
    Number number;
    Name name;
    Node *node;
}

%destructor { delete ($$); } <node>

/* BISON Declarations */
%define api.pure full
%define api.push-pull push

%parse-param {FlowController *flow} {bool *hasResult} {Node **result}

/* %error-verbose */

%token <number> NUMBER
%token <name> NAME

%left '='
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

%type <node> input expr lvalue

/* Grammar follows */
%%

input
    : expr '\n' { $$ = $1; *hasResult = true; *result = $1; YYACCEPT; }
    | '\n' { $$ = 0; *hasResult = false; YYACCEPT; }
    ;

expr
    : NUMBER                            { $$ = Node::createNumberLiteral($1);              }
    | NAME                              { $$ = Node::createName($1);                       }
    | lvalue '=' expr                   { $$ = Node::createAssignment($1, $3);             }
    | NAME '(' expr ')'                 { $$ = Node::createCall(Node::createName($1), $3); }
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

lvalue
    : NAME { $$ = Node::createName($1); }
    ;

newlines
    : '\n'
    | newlines '\n'
    ;

%%
