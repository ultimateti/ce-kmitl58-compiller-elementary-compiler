#include <stdint.h>

#define SEED 8699

/* symbol table */
struct symbol {             /* a variable name */
    char *name;
    int64_t value;
    uint8_t offset;
};

/* node types
 * + - * / %
 * M unary minus
 * E expression list
 * C IF statement
 * L LOOP statement่
 * ์V symbol ref
 * = assignment
 * K constant
 * S literal string
 * A Expression
 */

/* node in the abstraction syntax tree */
/* all have common initial nodetype */
struct ast {
    int nodetype;
    struct ast* l;
    struct ast* r;
};

struct cond {
    int nodetype;       /* type C */
    struct ast* fStmt;   /* condition */
    struct ast* sStmt;
    struct ast* tl;     /* then branch or do list */
};

struct loop {
    int nodetype;       /* type L */
    struct ast* from;   /* start number */
    struct ast* to;     /* end number */
    struct ast* tl;     /* then branch or do list */
};

struct lString {
    char* str;
    uint8_t label;
};

struct print {
    int nodetype;           /* type D H S */
    union arg {             /* argument of the print function */
        struct lString* ls;  /* string argument */
        struct ast* exp;    /* expression argument */
    } arg;
};

struct numval {
    int nodetype;       /* type N */
    int64_t number;
};

struct symref {
    int nodetype;       /* type V */
    struct symbol* s;
};

struct symasgn {
    int nodetype;       /* type = */
    struct symbol* s;
    struct ast* v;      /* value */
};

/* simple symtab of fixed size */
struct symbol symtab[SEED];

struct symbol* lookup(char*);

/* build an ast */
struct ast* newNode (struct ast* l, struct ast* r, int nodetype);
struct ast* newPrintStmt (struct ast *exp, char *str, int nodetype);
struct ast* newVar (struct symbol *s);
struct ast* newAssign (struct symbol *s, struct ast* v);
struct ast* newNum (int64_t num);
struct ast* newIfe (struct ast* firstStmt, struct ast* secStmt, struct ast* tl);
struct ast* newVon (struct ast* from, struct ast* to, struct ast* tl);

/* evaluate an ast */
int64_t eval (struct ast* );
void asmGen (struct ast* );

/* delete and free an ast */
void freeNode (struct ast* );

/* interface to the lexer */
extern int yylineno;    /* from lexer */
void yyerror (char *s, ...);


