#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ast.h"

struct cca_ast*
cca_ast_default()
{
    struct cca_ast *ast = malloc(sizeof(struct cca_ast));
    if (ast == NULL)
        return NULL;

    memset(ast, 0, sizeof(struct cca_ast));
    ast->next = NULL;
    return ast;
}

struct cca_ast_number*
cca_ast_default_number()
{
    struct cca_ast_number *num = malloc(sizeof(struct cca_ast_number));
    if (num == NULL)
        return NULL;
    return num;
}

struct cca_ast_infix*
cca_ast_default_infix()
{
    struct cca_ast_infix *bin = malloc(sizeof(struct cca_ast_infix));
    if (bin == NULL)
        return NULL;

    return bin;
}

struct cca_ast_prefix*
cca_ast_default_prefix()
{
    struct cca_ast_prefix *una = malloc(sizeof(struct cca_ast_prefix));
    if (una == NULL)
        return NULL;

    return una;
}

void
cca_ast_free_number(struct cca_ast_number *num)
{
    if (num == NULL)
        return;

    free(num);
}

void 
cca_ast_free_prefix(struct cca_ast_prefix *prefix)
{
    if (prefix == NULL)
        return;
    cca_ast_free(prefix->right);
    free(prefix);
}

void
cca_ast_free_infix(struct cca_ast_infix *infix)
{
    if (infix == NULL)
        return;

    cca_ast_free(infix->left);
    cca_ast_free(infix->right);
    free(infix);
}


void 
cca_ast_free(struct cca_ast *ast)
{

    if (ast == NULL)
        return;

    struct cca_ast *tmp = ast;
    while (tmp != NULL) {
        struct cca_ast *next = tmp->next;
        switch (tmp->ast_type) {
            case AST_NUMBER:
                cca_ast_free_number(tmp->ast.number);
                break;
            case AST_PREFIX:
                cca_ast_free_prefix(tmp->ast.prefix);
                break;
            case AST_INFIX:
                cca_ast_free_infix(tmp->ast.infix);
                break;
        }
        free(tmp);
        tmp = next;
    }
}
