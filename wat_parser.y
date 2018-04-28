%{
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <watcompiler.h>
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

%start program

%%

program: statement';' { (*(struct AstElement**)astDest) = $1; };

block: TOKEN_BEGIN statements TOKEN_END{ $$ = $2; };

statements: {$$=0;}
    | statements statement ';' {$$=makeStatement($1, $2);}
    | statements block';' {$$=makeStatement($1, $2);};

statement: 
      assignment {$$=$1;}
    | whileStmt {$$=$1;}
    | block {$$=$1;}
    | call {$$=$1;}

assignment: TOKEN_ID '=' expression {$$=makeAssignment($1, $3);}

expression: TOKEN_ID {$$=makeExpByName($1);}
    | TOKEN_NUMBER {$$=makeExpByNum($1);}
    | expression TOKEN_OPERATOR expression {$$=makeExp($1, $3, $2);}

whileStmt: TOKEN_WHILE expression TOKEN_DO statement{$$=makeWhile($2, $4);};

call: TOKEN_ID '(' expression ')' {$$=makeCall($1, $3);};
