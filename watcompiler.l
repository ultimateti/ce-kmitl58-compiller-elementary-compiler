%option noyywrap nodefault yylineno

%{
  #include "watcompiler.h"
  #include "watcompiler.tab.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <ctype.h>
  #include <string.h>

  static int64_t strToHex(char* s);
%}

%%

[ \t]             {  }
[0-9]+d           { yylval.num = (int64_t)atoi(yytext); return (NUM); }
[0-9a-fA-F]+[hH]  { yylval.num = strToHex(yytext); return (NUM);}
"_"[A-Z][A-Z]     { yylval.sym = lookup(yytext); return (VAR); }
["].*["]          { yylval.str = strdup(yytext); return (TEXT); }
"=>"              { return (TK_ASSIGN); }
"(" |
")" |
"{" |
"}" |
"+" |
"-" |
"*" |
"/" |
"%" |
"^" |
";" |
","               { return (yytext[0]); }
"prints"          { return (CMD_PS); }
"printh"          { return (CMD_PH); }
"printd"          { return (CMD_PD); }
"ife"             { return (TK_IFE); }
"von"             { return (TK_VON); }
\n                {  }
.                 { yyerror("Mystery character %c\n", *yytext); }
%%

static int64_t strToHex(char* s) {
  int len = strlen(s);
  s[len-1] = 0;
  return (int64_t)( strtol(s, NULL, 16) );
}


