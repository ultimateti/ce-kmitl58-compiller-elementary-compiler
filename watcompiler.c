/*
 * helper functions for calculator
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "watcompiler.h"

static uint8_t offsetCount = 0;
static uint8_t branchCount = 0;
static uint8_t litstrCount = 2;
static uint8_t symCount = 0;

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
  return yyparse();
}
