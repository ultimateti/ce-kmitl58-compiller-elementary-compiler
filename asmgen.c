#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  "asmgen.h"

#define FPS(A)  fputs(A, fp)

FILE* newFile(char* fname) {
  FILE *fp;
  fp = fopen(fname, "w+");
  return fp;
}

void closeFile(FILE* fp) {
  FPS("\tmovl\t$0, %eax\n");
  FPS("\tleave\n");
  FPS("\t.cfi_def_cfa 7, 8\n");
  FPS("\tret\n");
  FPS("\t.cfi_endproc\n");
  FPS(".LFE0:\n");
  FPS("\t.size main, .-main\n");
  FPS("\t.ident \"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.6) 5.4.0 20160609\"\n");
  FPS("\t.section\t.note.GNU-stack,\"\",@progbits\n");
	
  fclose(fp);
}

void putDataSec(FILE* fp, char dataSection[]) {
	//section.data
  FPS("\t.section .rodata\n");
  FPS(".LC0:\n");
  FPS("\t.string \"%ld\"\n");
  FPS(".LC1:\n");
  FPS("\t.string \"%Xh\"\n");
  FPS(".LC2:\n");
  FPS("\t.string \"%s\"\n");

	FPS(dataSection);
  free(dataSection);
}

void putBssSec(FILE* fp, char bssSection[]) {
  FPS("\t.file	\"test.c\"\n");
  FPS("\t.globl\tfrom\n");
  FPS("\t.bss\n");
  FPS("\t.align 8\n");
  FPS("\t.type\tfrom, @object\n");
  FPS("\t.size\tfrom, 8\n");
  FPS("from:\n");
  FPS("\t.zero\t8\n");
	FPS(bssSection);
}

void putTextSec(FILE* fp, char textSection[]) {
  FPS("\t.text\n");
  FPS("\t.globl\tmain\n");
  FPS("\t.type\tmain, @function\n");
  FPS("main:\n");
  FPS(".LFB0:\n");
  FPS("\t.cfi_startproc\n");
  FPS("\tpushq\t%rbp\n");
  FPS("\t.cfi_def_cfa_offset 16\n");
  FPS("\t.cfi_offset 6, -16\n");
  FPS("\tmovq\t%rsp, %rbp\n");
  FPS("\t.cfi_def_cfa_register 6\n");

  FPS(textSection);
	free(textSection);
}

char* genData(char s1[], char s2[]) {
	char* result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	free(s1);
	free(s2);

	return result;
}

char* genBss(char s1[], char s2[]) {
	char* result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	free(s1);
	free(s2);

	return result;
}

char* genText(char s1[], char s2[]) {
	char* result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	free(s1);
	free(s2);

	return result;
}

char* movq(char s1[], char s2[]) {
	char mv[] = "\tmovq\t";
	char comma[] = ", ";
	char* result = malloc(strlen(mv) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, mv);
  strcat(result, s1);
  strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");
	
  return result;
}

char* movl(char s1[], char s2[]) {
	char mv[] = "\tmovl\t";
	char comma[] = ", ";
	char* result = malloc(strlen(mv) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, mv);
  strcat(result, s1);
  strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");
	
  return result;
}

char* add(char* s1, char* s2) {
	char add[] = "\taddq\t ";
	char comma[] = ", ";
	char* result = malloc(strlen(add) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, add);
	strcat(result, s1);
	strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");

	return result;
}

char* cqto() {
  char* result = malloc(strlen("\tcqto\n"));
  strcpy(result, "\tcqto\n");
  
  return result;
}

char* sub(char* s1, char* s2) {
	char sub[] = "\tsubq\t ";
	char comma[] = ", ";
	char* result = malloc(strlen(sub) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, sub);
  strcat(result, s1);
  strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");
	
  return result;
}

char* imul(char* s1, char* s2) {
	char iml[] = "\timulq\t ";
	char comma[] = ", ";
	char* result = malloc(strlen(iml) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, iml);
  strcat(result, s1);
  strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");
	
  return result;
}

char* idiv(char* s1) {
	char idv[] = "\tidivq\t ";
	char* result = malloc(strlen(idv) + strlen("\n") + strlen(s1) + 1);
	strcpy(result, idv);
  strcat(result, s1);
	strcat(result, "\n");
	
  return result;
}

char* neg(char* s1) {
	char ng[] = "\tnegq\t ";
	char* result = malloc(strlen(ng) + strlen("\n") + strlen(s1) + 1);
	strcpy(result, ng);
  strcat(result, s1);
	strcat(result, "\n");
	
  return result;
}

char* cmp(char* s1, char* s2) {
	char cp[] = "\tcmpq\t ";
	char comma[] = ", ";
	char* result = malloc(strlen(cp) + strlen(comma) + strlen("\n") + strlen(s1) + strlen(s2) + 1);
	strcpy(result, cp);
  strcat(result, s1);
  strcat(result, comma);
	strcat(result, s2);
	strcat(result, "\n");
	
  return result;
}

char* push(char* s1) {
  char psh[] = "\tpushq\t ";
  char* result = malloc(strlen(psh) + strlen("\n") + strlen(s1) + 1);
  strcpy(result, psh);
  strcat(result, s1);
	strcat(result, "\n");
	
  return result;
}

char* pop(char* s1) {
  char pp[] = "\tpopq\t ";
  char* result = malloc(strlen(pp) + strlen("\n") + strlen(s1) + 1);
  strcpy(result, pp);
  strcat(result, s1);
	strcat(result, "\n");
	
  return result;
}

char* newString(char* lc, char* str) {
	char string[] = "\t.string";
	char* result = malloc(strlen(lc) + strlen(str) + strlen(string) + strlen("\"") + strlen("\"") + strlen(":") + 6);
	strcpy(result, ".");
	strcat(result, lc);
	strcat(result, ":\n");
	strcat(result, string);
	strcat(result, "\t");
	strcat(result, str);
	strcat(result, "\n");
	
  return result;
}

char* newLabel(char* label) {
  char* result = malloc(strlen(label) + strlen(":") + strlen(".") + 2);
  strcpy(result, ".");
  strcat(result, label);
  strcat(result, ":\n");

  return result;
}

char* newJump(char* mode, char* label) {
  char* result = malloc(strlen(label) + strlen(mode) + strlen(".") + 4);
  strcpy(result, "\t");
  strcat(result, mode);
  strcat(result, "\t");
  strcat(result, ".");
  strcat(result, label);
  strcat(result, "\n");
  
  return result;
}

char* syscallPrint(){
  char* pushC = "\tpushq\t%rcx\n";
	char* sysCall = "\tcall	printf\n";
  char* popC = "\tpopq\t%rcx\n";
	char* result = malloc(strlen(pushC) + strlen(sysCall) + strlen(popC) + 1);
  strcpy(result, pushC);
  strcat(result, sysCall);
  strcat(result, popC);
	
  return result;
}
