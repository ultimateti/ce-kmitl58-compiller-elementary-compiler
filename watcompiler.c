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

int errorflag;

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
  int scount = SEED;

  while (--scount >= 0) {
    if (sp->name && !strcmp(sp->name, sym)) {
      return sp;
    }

    // new entry
    if (!sp->name) {    
      sp->name = strdup(sym);
      sp->offset = 0;

      ++symCount;
      return sp;
    }

    // if hash collition happens
    if (++sp >= symtab + SEED){
      sp = symtab;   
    }
  }

  // if hash table is full
  yyerror("symbol table overflow\n");
  abort();
}

// create new node in ast
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

// create new constant number
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

// create new print node
struct ast* newPrintStmt (struct ast *exp, char *str, int nodetype) {
  struct print *tmp = (struct print*) malloc(sizeof(struct print));
  struct lString *ls = (struct lString*) malloc(sizeof(struct lString));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = nodetype;

  // if we desire to create print decimal or hex
  if (nodetype == 'D' || nodetype == 'H') {
    tmp->arg.exp = exp;
  } else {
    // else we wants to create print string
    char buf[21];

    // create new string and put into data section
    sprintf(buf, "LC%u", litstrCount);
    data = genData(data, newString(buf, str));

    ls->str = str;
    ls->label = litstrCount++;
    tmp->arg.ls = ls;
  }

  return (struct ast*) tmp;
}

// create new variable node
struct ast* newVar (struct symbol *s) {

  // allocate in hash table
  struct symref *tmp = (struct symref*) malloc(sizeof(struct symref));

  if (!tmp) {
    yyerror("out of space");
    exit(0);
  }

  tmp->nodetype = 'V';
  tmp->s = s;

  return (struct ast*) tmp;
}

// create new variable assignment node
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

// create new condition node
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

// create new loop node
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

// free a tree of ASTs 
void freeNode (struct ast* node) {
  switch (node->nodetype) {
    // node with two subrtrees
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case 'B':
      freeNode(node->r);

    // node with one subtree
    case '^':
      freeNode(node->l);

    // node with no subtree
    case 'N':
    case 'V':
      break;

    // for print node, we need to free expression in
    // command too
    case 'D':
    case 'H':
      freeNode(((struct print*)node)->arg.exp);
      break;

    // for print string node, we need to free string
    case 'S':
      free(((struct print*)node)->arg.ls->str);
      free(((struct print*)node)->arg.ls);
      break;

    // assignment node
    case '=':
      freeNode(((struct symasgn*)node)->v);
      break;

    // condition node
    case 'C':
      freeNode(((struct cond*)node)->fStmt);
      freeNode(((struct cond*)node)->sStmt);

      if (((struct cond*)node)->tl)
        freeNode(((struct cond*)node)->tl);

      break;

    // loop node
    case 'L':
      freeNode(((struct loop*)node)->from);
      freeNode(((struct loop*)node)->to);

      if (((struct loop*)node)->tl)
        freeNode(((struct loop*)node)->tl);

      break;

    default:
      printf("internal error: free bad node %c\n", node->nodetype);
  }

  // finally, free node itself 
  free(node);
}

// evaluate number or statement
int64_t eval (struct ast* node) {
  int64_t v;

  if (!node) {
      yyerror("internal error, null eval");

      return 0;
  }

  switch (node->nodetype) {
    // constant
    case 'N':
      v = ((struct numval*)node)->number;
      break;

    // expressions
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

    default:
      printf("internal error: bad node %c\n", node->nodetype);
  }

  return v;
}

// generate ASM code
void asmGen (struct ast* node) {
  struct symbol *s;
  char buf[21], tmpbuf[21];
  uint8_t condBranchNum, loopBranchNum;

  if (!node) {
    yyerror("internal error, null node");
    return;
  }

  switch (node->nodetype) {
    // constant number
    case 'N':
      text = genText(text, sub("$8", "%rsp"));
      sprintf(buf, "$%ld", ((struct numval*)node)->number);
      text = genText(text, movq(buf, "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));
      break;
      
    // variable reference
    case 'V':
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

    // print decimal
    case 'D':
      asmGen(((struct print*)node)->arg.exp);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, movq("%rax", "%rsi"));
      text = genText(text, movl("$.LC0", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;

    // print hexadecimal
    case 'H':
      asmGen(((struct print*)node)->arg.exp);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, movq("%rax", "%rsi"));
      text = genText(text, movl("$.LC1", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;
        
    // print string
    case 'S':
      sprintf(buf, "$.LC%u", ((struct print*)node)->arg.ls->label);

      text = genText(text, movl(buf, "%esi"));
      text = genText(text, movl("$.LC2", "%edi"));
      text = genText(text, movl("$0", "%eax"));
      text = genText(text, syscallPrint());

      break;

        
    // variable assignment
    case '=':
      s = lookup(((struct symasgn*)node)->s->name);
      // if assign to new variable, find an offset for it
      if (s->offset == 0) {
        s->offset = (++offsetCount) * 8;
      }
      asmGen(((struct symasgn*)node)->v);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      sprintf(buf, "-%u(%%rbp)", s->offset);
      text = genText(text, movq("%rax", buf));
      text = genText(text, sub("$8", "%rsp"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    // expressions
    case '+':
      asmGen(node->r);
      asmGen(node->l);

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

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, sub("(%rsp)", "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    case '*':
      asmGen(node->r);
      asmGen(node->l);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, imul("(%rsp)", "%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;
      
    case '/':
      asmGen(node->r);
      asmGen(node->l);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, cqto());
      text = genText(text, idiv("(%rsp)"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    case '%':
      asmGen(node->r);
      asmGen(node->l);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, add("$8", "%rsp"));
      text = genText(text, cqto());
      text = genText(text, idiv("(%rsp)"));
      text = genText(text, movq("%rdx", "(%rsp)"));

      break;

    case '^':
      asmGen(node->l);

      text = genText(text, movq("(%rsp)", "%rax"));
      text = genText(text, neg("%rax"));
      text = genText(text, movq("%rax", "(%rsp)"));

      break;

    // condition
    case 'C':
      // need number to assign to label for jumping
      condBranchNum = branchCount++;

      asmGen(((struct cond*)node)->sStmt);
      asmGen(((struct cond*)node)->fStmt);

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

    // loop
    case 'L':
      if (((struct loop*)node)->tl) {

        // need number to assign to label for jumping
        loopBranchNum = branchCount; 
        branchCount += 2;
        
        // keep iteration number in %rcx
        text = genText(text, push("%rcx"));
        
        // if argument 'from' is variable we need to resolve it first
        if(((struct loop*)node)->from->nodetype == 'V') {
          struct symasgn* tmp = (struct stmasgn*)(((struct loop*)node)->from);
          s = lookup(tmp->s->name);
          sprintf(buf, "-%u(%%rbp)", s->offset);
        } else {
          // eles just put a number
          sprintf(buf, "$%ld", eval(((struct loop*)node)->from));
        }
        
        // use jump label as variable to store iteration count
        sprintf(tmpbuf, "L%u", loopBranchNum);
        s = lookup(tmpbuf);
        if (s->offset == 0) {
          s->offset = (++offsetCount) * 8;
        }
        sprintf(tmpbuf, "-%u(%%rbp)", s->offset);
        
        text = genText(text, (movq(buf, "%rcx")));
        text = genText(text, (movq("%rcx", tmpbuf)));
        sprintf(buf, "L%u", loopBranchNum);
        text = genText(text, newJump("jmp", buf));
        sprintf(buf, "L%u", loopBranchNum + 1);
        text = genText(text, newLabel(buf));

        // evaluate loop body
        asmGen(((struct loop*)node)->tl);
        text = genText(text, movq(tmpbuf, "%rcx"));
        text = genText(text, add("$1", "%rcx"));
        text = genText(text, movq("%rcx", tmpbuf));
        // if argument 'from' is variable, update it as well
        if(((struct loop*)node)->from->nodetype == 'V') {
          struct symasgn* tmp = (struct stmasgn*)(((struct loop*)node)->from);
          s = lookup(tmp->s->name);
          sprintf(buf, "-%u(%%rbp)", s->offset);
          text = genText(text, add("$1", buf));
        }
        
        sprintf(buf, "L%u", loopBranchNum);
        text = genText(text, newLabel(buf));

        // if argument 'to' is variable, we need to resolve it too
        if(((struct loop*)node)->to->nodetype == 'V') {
          struct symasgn* tmp = (struct stmasgn*)(((struct loop*)node)->to);
          s = lookup(tmp->s->name);
          sprintf(buf, "-%u(%%rbp)", s->offset);
        } else {
          sprintf(buf, "$%ld", eval(((struct loop*)node)->to));
        }
        text = genText(text, cmp(buf, "%rcx"));
        sprintf(buf, "L%u", loopBranchNum + 1);
        text = genText(text, newJump("jl", buf)); 
        text = genText(text, pop("%rcx"));
      }

      break;

    // block statements
    case 'B':
      asmGen(node->l);
      asmGen(node->r);
      break;

    default:
      printf("internal error: bad node %c\n", node->nodetype);
  }

}

// print error
void yyerror (char *s, ...) {
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char **argv) {
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

  // create new file
  asmFile = strdup(argv[1]);
  ptr = strchr(asmFile, '.');

  *(ptr+1) = 's';
  *(ptr+2) = '\0';

  printf("Start Parsing ...\n");
  printf("...\n");
  printf("...\n");
  yyparse();
  if (errorflag) {
    printf("\nParsing Error\n");
    return -1;
  }

  fp = newFile(asmFile);

  // print symbol count to file
  sprintf(buf, "$%u", (symCount + 1) * 8);
  text = genText(sub(buf, "%rsp"), text);
  // put Bss, Data, Text section to ASM code
  putBssSec(fp, "");
  putDataSec(fp, data);
  putTextSec(fp, text);

  // close file
  closeFile(fp);

  printf("Finished Parsing\n");

  return 0;
}
