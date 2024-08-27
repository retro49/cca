#include <stdio.h>
#include <stdlib.h>

#include "token.h"

#ifndef _CCA_AST_H
#define _CCA_AST_H

enum cca_ast_type {
    AST_NUMBER = 0,
    AST_PREFIX,
    AST_INFIX,
};

struct cca_ast_number {
    struct cca_token *tkn;
    double number;
};

struct cca_ast_prefix {
    struct cca_token *tkn;
    char symbol;
    struct cca_ast *right;
};

struct cca_ast_infix {
    struct cca_token *tkn;
    char symbol;
    struct cca_ast *left;
    struct cca_ast *right;
};

struct cca_ast {
    enum cca_ast_type ast_type;

    union {
        struct cca_ast_number *number;
        struct cca_ast_prefix *prefix;
        struct cca_ast_infix *infix;
    }ast;

    struct cca_ast *next;
};

struct cca_ast *cca_ast_default();

struct cca_ast_number *cca_ast_default_number();
struct cca_ast_infix *cca_ast_default_infix();
struct cca_ast_prefix *cca_ast_default_prefix();

void cca_ast_free(struct cca_ast*);

#endif
