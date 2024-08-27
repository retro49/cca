#include <stdio.h>
#include <stdlib.h>

#include "token.h"

#ifndef _CCA_AST_H
#define _CCA_AST_H

enum cca_ast_type {
    AST_NUMBER = 0,
    AST_UNARY,
    AST_BINARY,
};

struct cca_ast_number {
    struct cca_token *tkn;
};

struct cca_ast_unary {
    struct cca_token *symbol;
    void* right;
};

struct cca_ast_binary {
    struct cca_token *symbol;
    void *left;
    void *right;
};

struct cca_ast {
    enum cca_ast_type ast_type;
    union {
        struct cca_ast_number *number;
        struct cca_ast_unary *unary;
        struct cca_ast_binary *binary;
    };

    struct cca_ast *next;
};

#endif
