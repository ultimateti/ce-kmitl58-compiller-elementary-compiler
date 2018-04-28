%option noyywrap nodefault yylineno

%{
  #include "watlang.tab.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <ctype.h>
  #include <string.h>

  int strToHex(char* s);
  int getVar(char* s);
  char* stringify(char* s);
%}

%%

[ \t]             {  }
[0-9]+d           { yylval.num = (int64_t)atoi(yytext); return (NUM); }
[0-9a-fA-F]+[hH]  { yylval.num = (int64_t)strToHex(yytext); return (NUM);}
"_"[A-Z][A-Z]     { yylval.sym = (int64_t)getVar(yytext); return (VAR); }
["].*["]          { yylval.str = stringify(yytext); return (TEXT); }
\n                { return (TK_NEWLINE); }
"=>"              { return (TK_ASSIGN); }
[(){}+-*/%^]      { return (yytext[0]); }
"prints"          { return (CMD_PS); }
"printh"          { return (CMD_PH); }
"printd"          { return (CMD_PD); }
"ife"             { return (TK_IFE); }
%%

int strToHex(char* s) {
  int len = strlen(s);
  s[len-1] = 0;
  return (int)( strtol(s, NULL, 16) );
}

int getVar(char* s) {
  return (((s[1] - 'A') * 10) + (s[2] - 'A'));
}

char* stringify(char* s) {
  int len = strlen(s);

  int ts = 1, rs = 0;
  int lastQuote = len - 1;
  char* ret_s = malloc(sizeof(char) * (len - 1));
  while(ts != lastQuote) {
    if (ts + 1 != lastQuote && s[ts] == '\\') {
      switch(s[ts + 1]) {
        case 'n': ret_s[rs++] = '\n'; break;
        case 't': ret_s[rs++] = '\t'; break;
        case '"': ret_s[rs++] = '"'; break;
        case '\\': ret_s[rs++] = '\\'; break;
      }
      ts+=2;
    } else {
      ret_s[rs++] = s[ts++];
    }
  }

  ret_s[rs] = '\0';
  return ret_s;
}


