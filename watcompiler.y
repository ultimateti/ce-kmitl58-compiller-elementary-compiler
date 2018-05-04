%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "watcompiler.h"

extern int errorflag;

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
%nonassoc NEG

%type <node> stmt exp term block

%start program

%%

program:
| program stmt      {   
                      asmGen($2);
                      freeNode($2);
                    }
| program error ';' { errorflag = 1; yyerrok; }
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
| CMD_PS TEXT               { $$ = newPrintStmt(NULL, $2, 'S'); }
| CMD_PD exp                { $$ = newPrintStmt($2, NULL, 'D'); }
| CMD_PH exp                { $$ = newPrintStmt($2, NULL, 'H'); }
;

term:
  NUM                       { $$ = newNum($1); }
| VAR                       { $$ = newVar($1); }
;

block:
                            { $$ = NULL; }
| stmt block                { 
                              if ($2 == NULL) {
                                $$ = $1;
                              } else {
                                $$ = newNode($1, $2, 'B');
                              }
                            }
;