/*
 * helper functions for calculator
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "watcompiler.h"
#include "asmgen.h"

static uint8_t offsetCount = 0;
static uint8_t branchCount = 0;
static uint8_t litstrCount = 3;
static uint8_t symCount = 0;

static FILE *fp;
static char *text;
static char *data;

/* symbol table */
/* hash a symbol */
static unsigned symhash (char* sym) {
  unsigned int hash = 0;
  unsigned c;
  
  while ((c = (*(sym++)))) 
    hash = hash * 7 ^ c;

  return hash;
}

struct symbol* lookup (char* sym) {
  struct symbol* sp = &symtab[symhash(sym) % SEED];
  int scount = SEED;     /* how many have we looked at */

  while (--scount >= 0) {
    if (sp->name && !strcmp(sp->name, sym)) {
      return sp;
    }

    if (!sp->name) {    /* new entry */
      sp->name = strdup(sym);
      sp->value = 0;
      sp->offset = 0;

      ++symCount;
      return sp;
    }

    if (++sp >= symtab + SEED){
      sp = symtab;    /* try the next entry */
    }
  }

  yyerror("symbol table overflow\n");
  abort(); /* tried them all, table is full */
}

struct ast* newNode (struct ast* l, struct ast* r, int nodetype) {
  struct ast* tmp = (struct ast*) malloc(sizeof(struct ast));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = nodetype;
  tmp->l = l;
  tmp->r = r;

  return tmp;
}

struct ast* newNum (int64_t num) {
    struct numval* tmp = (struct numval*) malloc(sizeof(struct numval));

    if (!tmp) {
        yyerror("out of space");
        exit(0);
    }

    tmp->nodetype = 'N';
    tmp->number = num;

    return (struct ast*) tmp;
}

struct ast* newPrintStmt (struct ast *exp, char *str, int nodetype) {
  struct print *tmp = (struct print*) malloc(sizeof(struct print));
  struct lString *ls = (struct lString*) malloc(sizeof(struct lString));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = nodetype;

  if (nodetype == 'D' || nodetype == 'H') {
    tmp->arg.exp = exp;
  } else {
    char buf[21];

    sprintf(buf, "LC%u", litstrCount);
    data = genData(data, newString(buf, str));

    ls->str = str;
    ls->label = litstrCount++;
    tmp->arg.ls = ls;
  }

  return (struct ast*) tmp;
}

struct ast* newVar (struct symbol *s) {
  struct symref *tmp = (struct symref*) malloc(sizeof(struct symref));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = 'V';
  tmp->s = s;

  return (struct ast*) tmp;
}

struct ast* newAssign (struct symbol *s, struct ast* v) {
  struct symasgn *tmp = (struct symasgn*) malloc(sizeof(struct symasgn));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = '=';
  tmp->s = s;
  tmp->v = v;

  return (struct ast*) tmp;
}

struct ast* newIfe (struct ast* firstStmt, struct ast* secStmt, struct ast* tl) {
  struct cond *tmp = (struct cond*) malloc(sizeof(struct cond));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = 'C';
  tmp->fStmt = firstStmt;
  tmp->sStmt = secStmt;
  tmp->tl = tl;

  return (struct ast*) tmp;
}

struct ast* newVon (struct ast* from, struct ast* to, struct ast* tl) {
  struct loop* tmp = (struct loop*) malloc(sizeof(struct loop));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = 'L';
  tmp->from = from;
  tmp->to = to;
  tmp->tl = tl;

  return (struct ast*) tmp;
}

/* free a tree of ASTs */
void freeNode (struct ast* node) {
  switch (node->nodetype) {
    /* two subtrees */
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case 'B':
      freeNode(node->r);

    /* one subtrees */
    case '^':
      freeNode(node->l);

    /* no subtree */
    case 'N':
    case 'V':
      break;

    /* print expression */
    case 'D':
    case 'H':
      freeNode(((struct print*)node)->arg.exp);
      break;

    /* print string */
    case 'S':
      free(((struct print*)node)->arg.ls->str);
      free(((struct print*)node)->arg.ls);
      break;

    /* assignment */
    case '=':
      freeNode(((struct symasgn*)node)->v);
      break;

    /* condtion */
    case 'C':
      freeNode(((struct cond*)node)->fStmt);
      freeNode(((struct cond*)node)->sStmt);

      if (((struct cond*)node)->tl)
        freeNode(((struct cond*)node)->tl);

      break;

    /* loop */
    case 'L':
      freeNode(((struct loop*)node)->from);
      freeNode(((struct loop*)node)->to);

      if (((struct loop*)node)->tl)
        freeNode(((struct loop*)node)->tl);

      break;

    default:
      printf("internal error: free bad node %c\n", node->nodetype);
  }

  free(node); /*always free the node itself */
}

int64_t eval (struct ast* node) {
  int64_t v;
  //int64_t count;

  if (!node) {
      yyerror("internal error, null eval");

      return 0;
  }

  switch (node->nodetype) {
    /* constant */
    case 'N':
      v = ((struct numval*)node)->number;
      break;
      
    /*num reference */
    case 'V':
      v = ((struct symref*)node)->s->value;
      break;

    /* print expression */
    case 'D':
      v = eval(((struct print*)node)->arg.exp);
      printf("%ld", v);
      break;

    case 'H':
      v = eval(((struct print*)node)->arg.exp);
      printf("%Xh", v);
      break;
        
    /* print literal string */
    case 'S':
      printf("%s", (((struct print*)node)->arg.ls->str));
      break;
        
    /* assignment */
    case '=':
      v = ((struct symasgn*)node)->s->value = eval(((struct symasgn*)node)->v);
      break;

    /* expressions */
    case '+':
      v = eval(node->l) + eval(node->r);
      break;
    case '-':
      v = eval(node->l) - eval(node->r);
      break;
    case '*':
      v = eval(node->l) * eval(node->r);
      break;
    case '/':
      v = eval(node->l) / eval(node->r);
      break;
    case '%':
      v = eval(node->l) % eval(node->r);
      break;
    case '^':
      v = -eval(node->l);
      break;

    /* control flow */
    /* null expressions allowed in the grammar, so check for them */

    /* condition */
    case 'C':
      v = 0; /* a default value */

      if (eval(((struct cond*)node)->fStmt) == eval(((struct cond*)node)->sStmt)) {
        if (((struct cond*)node)->tl)
          v = eval(((struct cond*)node)->tl);
      }

      break;

    /* loop */
    case 'L':
      v = 0; /* a default value */

      if (((struct loop*)node)->tl) {
        int64_t count = eval(((struct loop*)node)->from);
        int64_t to = eval(((struct loop*)node)->to);

        while (count < to) {
          v = eval(((struct loop*)node)->tl);
          count++;
        }
      }

      break;

    /* list of statements */
    case 'B':
      eval(node->l);
      v = eval(node->r);
      break;

    default:
      printf("internal error: bad node %c\n", node->nodetype);
  }

  return v;
}

void asmGen (struct ast* node) {
  struct symbol *s;
  char buf[21];
  uint8_t condBranchNum, loopBranchNum;

  if (!node) {
    yyerror("internal error, null node");
    return;
  }

  switch (node->nodetype) {
    /* constant */
    case 'N':
      // printf("\n# gen N\n");
      text = genText(text, sub("$8", "%rsp"));
      sprintf(buf, "$%ld", ((struct numval*)node)->number);
      text = genText(text, movl(buf, "%eax"));
      text = genText(text, movq("%rax", "(%rsp)"));
      break;
      
    /*num reference */
    case 'V':
      // printf("\n# gen V\n");
      s = lookup(((struct symasgn*)node)->s->name);

      if (s->offset == 0) {
        s->offset = (++offsetCount) * 8;
        sprintf(buf, "-%u(%%rbp)", s->offset);
        text = genText(text, movq("$0", buf));
      }

      sprintf(buf, "-%u(%%rbp)", s->offset);
      text = genText(text, movq(buf, "%rax"));
      text = genText(text, sub("$8", "%rsp"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    /* print expression */
    case 'D':
      asmGen(((struct print*)node)->arg.exp);

      // printf("\n# print D\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, movq("%rax", "%rsi"));
      text = genText(text, movl("$.LC0", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;

    case 'H':
      asmGen(((struct print*)node)->arg.exp);

      // printf("\n# print H\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, movq("%rax", "%rsi"));
      text = genText(text, movl("$.LC1", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;
        
    /* print literal string */
    case 'S':
      // printf("\n# print S\n");
      sprintf(buf, "$.LC%u", ((struct print*)node)->arg.ls->label);

      text = genText(text, movl(buf, "%esi"));
      text = genText(text, movl("$.LC2", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;

        
    /* assignment */
    case '=':
      s = lookup(((struct symasgn*)node)->s->name);
      if (s->offset == 0) {
        s->offset = (++offsetCount) * 8;
      }
      asmGen(((struct symasgn*)node)->v);

      // printf("\n# assignment\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      sprintf(buf, "-%u(%%rbp)", s->offset);
      text = genText(text, movq("%rax", buf));
      text = genText(text, sub("$8", "%rsp"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    /* expressions */
    case '+':
      asmGen(node->r);
      asmGen(node->l);

      // printf("\n# addition\n");
      text = genText(text, movq("(%rsp)", "%rdx"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, add("%rdx", "%rax"));
      text = genText(text, sub("$8", "%rsp"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    case '-':
      asmGen(node->r);
      asmGen(node->l);

      // printf("\n# subtraction\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, sub("(%rsp)", "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    case '*':
      asmGen(node->r);
      asmGen(node->l);

      // printf("\n# multiplication\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, imul("(%rsp)", "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;
      
    case '/':
      asmGen(node->r);
      asmGen(node->l);

      // printf("\n# division\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, cqto());
      text = genText(text, idiv("(%rsp)"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    case '%':
      asmGen(node->r);
      asmGen(node->l);

      // printf("\n# modulation\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, cqto());
      text = genText(text, idiv("(%rsp)"));
      text = genText(text, movq("%rdx", "(%rsp)"));

      break;

    case '^':
      asmGen(node->l);

      // printf("\n# Negation\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, neg("%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    /* control flow */
    /* null expressions allowed in the grammar, so check for them */

    /* condition */
    case 'C':
      condBranchNum = branchCount++;

      // printf("\n# Condition\n");
      asmGen(((struct cond*)node)->sStmt);
      asmGen(((struct cond*)node)->fStmt);

      // printf("\n# subtraction\n");
      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, sub("(%rsp)", "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));
      
      
      text = genText(text, cmp("$0", "(%rsp)"));
      sprintf(buf, "L%u", condBranchNum);
      text = genText(text, newJump("jne", buf));

      if (((struct cond*)node)->tl)
          asmGen(((struct cond*)node)->tl);

      sprintf(buf, "L%u", condBranchNum);
      text = genText(text, newLabel(buf));
      text = genText(text, add("$8", "%rsp"));

      break;

    /* loop */
    case 'L':
      if (((struct loop*)node)->tl) {
        // printf("\n# Loop\n");
        
        loopBranchNum = branchCount; //1
        branchCount += 2;
        
        text = genText(text, push("%rcx"));

        sprintf(buf, "$%ld", eval(((struct loop*)node)->from));
        text = genText(text, (movq(buf, "%rcx")));
        sprintf(buf, "L%u", loopBranchNum);
        text = genText(text, newJump("jmp", buf));
        sprintf(buf, "L%u", loopBranchNum + 1);
        text = genText(text, newLabel(buf));

        asmGen(((struct loop*)node)->tl);

        text = genText(text, add("$1", "%rcx"));
        sprintf(buf, "L%u", loopBranchNum);
        text = genText(text, newLabel(buf));
        sprintf(buf, "$%ld", eval(((struct loop*)node)->to) - 1);
        text = genText(text, cmp(buf, "%rcx"));
        sprintf(buf, "L%u", loopBranchNum + 1);
        text = genText(text, newJump("jle", buf));
        
        text = genText(text, pop("%rcx"));
      }

      break;

    /* list of statements */
    case 'B':
      asmGen(node->l);
      asmGen(node->r);
      break;

    default:
      printf("internal error: bad node %c\n", node->nodetype);
  }

}

void yyerror (char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
  extern FILE *yyin;
  char *asmFile;
  char *ptr;
  char buf[21];

  if (argc > 1) {
      if (!(yyin = fopen(argv[1], "r"))) {
          perror(argv[1]);
          return 1;
      }
  }
  else {
      printf("\nPlease specify a source file.\n\n");
      return 1;
  }

  text = (char *) malloc(sizeof(char));
  data = (char *) malloc(sizeof(char));
  *(text) = '\0';
  *(data) = '\0';

  asmFile = strdup(argv[1]);
  ptr = strchr(asmFile, '.');

  *(ptr+1) = 's';
  *(ptr+2) = '\0';

  fp = newFile(asmFile);

  printf("Start Parsing ...\n");
  printf("...\n");
  printf("...\n");

  if (yyparse()) {
    printf("\nParsing Error\n");
    // closeFile(fp);
    return -1;
  }

  sprintf(buf, "$%u", (symCount + 1) * 8);
  text = genText(sub(buf, "%rsp"), text);
  putBssSec(fp, "");
  putDataSec(fp, data);
  putTextSec(fp, text);
  closeFile(fp);

  printf("Finished Parsing\n");

  return 0;
}
