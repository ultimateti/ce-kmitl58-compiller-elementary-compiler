#ifndef ASMGEN_INCLUDED
#define ASMGEN_INCLUDED

FILE* newFile(char* name);
void closeFile(FILE* fp);
void putDataSec(FILE* fp, char dataSection[]);
void putBssSec(FILE* fp, char bssSection[]);
void putTextSec(FILE* fp, char textSection[]);
char* genData(char s1[], char s2[]);
char* genBss(char s1[], char s2[]);
char* genText(char s1[], char s2[]);
char* movq(char s1[], char s2[]);
char* movl(char s1[], char s2[]);
char* add(char* s1, char* s2);
char* sub(char* s1, char* s2);
char* cqto(void);
char* imul(char* s1, char* s2);
char* idiv(char* s1);
char* neg(char* s1);
char* cmp(char* s1, char* s2);
char* newString(char* lc, char* str);
char* newLabel(char* label);
char* newJump(char* mode, char* label);
char* syscallPrint();

#endif // ASMGEN_INCLUDED
