%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "watcompiler.h"
%}

%union {
    struct ast* node;
    struct symbol* sym;
    int64_t num;
    char* str;
}


%token<str> TEXT
%token<num> NUM
%token<sym> VAR
%token TK_ASSIGN
%token TK_NEWLINE
%token TK_IFE TK_VON CMD_PS CMD_PD CMD_PH

%right TK_ASSIGN
%left '+' '-'
%left '*' '/' '%'
%nonassoc NEG   /* negation--unary minus */

%type <node> stmt exp term block

%start program

%%

program:
| program stmt TK_NEWLINE   {   
                                eval($2);
                                freeNode($2);
                            }
| program error TK_NEWLINE    { yyerrok; }
;

stmt:
  exp ';'
| TK_IFE '(' exp ',' exp ')' '{' block '}'      { $$ = newIfe($3, $5, $8); }
| TK_VON '(' exp ',' exp ')' '{' block '}'      { $$ = newVon($3, $5, $8); }
;

exp:
  term
| VAR TK_ASSIGN exp         { $$ = newAssign($1, $3); }
| exp '+' exp               { $$ = newNode($1, $3, '+'); }
| exp '-' exp               { $$ = newNode($1, $3, '-'); }
| exp '*' exp               { $$ = newNode($1, $3, '*'); }
| exp '/' exp               { $$ = newNode($1, $3, '/'); }
| exp '%' exp               { $$ = newNode($1, $3, '%'); }
| '^' exp %prec NEG         { $$ = newNode($2, NULL, '^'); }
| '(' exp ')'               { $$ = $2; }
| CMD_PS '(' TEXT ')'       { $$ = newPrintStmt(NULL, $3, 'S'); }
| CMD_PD '(' exp ')'        { $$ = newPrintStmt($3, NULL, 'D'); }
| CMD_PH '(' exp ')'        { $$ = newPrintStmt($3, NULL, 'H'); }
;

term:
  NUM                       { $$ = newNum($1); }
| VAR                       { $$ = newVar($1); }
;

block:
                            { $$ = NULL; }
| exp ';' block             { 
                                if ($3 == NULL) {
                                    $$ = $1;
                                } else {
                                    $$ = newNode($1, $3, 'B');
                                }
                            }
;