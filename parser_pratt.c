#include <stdbool.h>
#include <stdlib.h>

#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "parser_pratt.h"


const enum cca_token_types 
CCA_PP_PREFIX_TABLE[CCA_PP_PREFIX_TABLE_SIZE] = {
    TK_NUMBER,
    TK_MINUS,
    TK_LPAR
};

const enum cca_token_types 
CCA_PP_INFIX_TABLE[CCA_PP_INFIX_TABLE_SIZE]  = {
    TK_PLUS,
    TK_MINUS,
    TK_ASTERISK,
    TK_SLASH
};

struct cca_parser_pratt*
cca_pp_new(struct cca_lexer *lxr)
{
    if (lxr == NULL) {
        return NULL;
    }

    struct cca_parser_pratt *prsr = malloc(sizeof(struct cca_parser_pratt));
    if (prsr == NULL){
        return NULL;
    }

    prsr->lxr = lxr;
    prsr->_tokens = cca_lexer_lex(lxr);
    prsr->_tokens_tmp = prsr->_tokens;
    cca_pp_next(prsr);

    return prsr;
}

void
cca_pp_next(struct cca_parser_pratt *prsr)
{
    if (prsr == NULL)
        return;


    if (prsr->_tokens_tmp == NULL) {
        prsr->tkn = NULL;
        prsr->peek_tkn == NULL;
        return;
    }
    prsr->tkn = prsr->_tokens_tmp;

    prsr->_tokens_tmp = prsr->_tokens_tmp->next;
    prsr->peek_tkn = prsr->_tokens_tmp;
}

struct cca_ast*
cca_pp_parse_infix(struct cca_parser_pratt *prsr, struct cca_ast *left)
{
    struct cca_ast *ast = cca_ast_default();
    if (ast == NULL)
        return NULL;
    struct cca_ast_infix *bin = cca_ast_default_infix();
    if (bin == NULL) {
        if (ast != NULL)
            free(ast);

        return NULL;
    }


    bin->left = left;
    bin->symbol = prsr->tkn->token_literal.symbol;
    bin->tkn = prsr->tkn;
    enum cca_pp_precedence pr = cca_pp_peek_precedence(prsr);
    cca_pp_next(prsr);
    bin->right = cca_pp_parse_expression(prsr, pr);

    ast->ast_type  = AST_INFIX;
    ast->ast.infix = bin;

    return ast;
}


struct cca_ast*
cca_pp_parse_number(struct cca_parser_pratt *prsr)
{
    struct cca_ast *ast = cca_ast_default();
    if (ast == NULL)
        return NULL;

    struct cca_ast_number *num = cca_ast_default_number();
    if (num == NULL) {
        if (ast != NULL)
            free(ast);

        return NULL;
    }

    num->number = prsr->tkn->number;
    num->tkn = prsr->tkn;
    num->number = prsr->tkn->number;

    ast->ast_type = AST_NUMBER;
    ast->ast.number = num;
    return ast;
}

struct cca_ast*
cca_pp_parse_prefix_unary(struct cca_parser_pratt *prsr)
{
    struct cca_ast *ast = cca_ast_default();
    if (ast == NULL)
        return NULL;

    struct cca_ast_prefix *prefix = cca_ast_default_prefix();
    if (prefix == NULL) {
        if (ast != NULL)
            free(ast);

        return NULL;
    }

    prefix->symbol = prsr->tkn->token_literal.symbol;
    prefix->tkn =  prsr->tkn;
    cca_pp_next(prsr);
    prefix->right = cca_pp_parse_expression(prsr, PR_PREFIX);

    ast->ast_type = AST_PREFIX;
    ast->ast.prefix = prefix;
    return ast;
}

struct cca_ast*
cca_pp_parse_prefix(struct cca_parser_pratt *prsr)
{
    switch (prsr->tkn->token_type) {
        case TK_NUMBER:
            return cca_pp_parse_number(prsr);
        case TK_MINUS:
            return cca_pp_parse_prefix_unary(prsr);
        case TK_LPAR:
            cca_pp_next(prsr);
            struct cca_ast* result = cca_pp_parse_expression(prsr, PR_LOW);
            cca_pp_next(prsr);
            if (prsr->tkn->token_type != TK_RPAR) {
                printf("expected closing parenthesis ')'");
                return NULL;
            }
            return result;
        default:
            return NULL;
    }
}


struct cca_ast*
cca_pp_parse_expression(struct cca_parser_pratt *prsr, 
                        enum cca_pp_precedence precedence)
{
    if (prsr == NULL)
        return NULL;

    if (!cca_pp_prefix_has(prsr->tkn->token_type))
        return NULL;

    struct cca_ast* prefix = cca_pp_parse_prefix(prsr);

    enum cca_pp_precedence pr = cca_pp_peek_precedence(prsr);
    while (prsr->peek_tkn->token_type != TK_SEMICOLON && pr > precedence) {
        if (!cca_pp_infix_has(prsr->peek_tkn->token_type))
            return prefix;

        cca_pp_next(prsr);
        prefix = cca_pp_parse_infix(prsr, prefix);
    }

    return prefix;
}

struct cca_ast*
cca_pp_parse(struct cca_parser_pratt *prsr)
{
    if (prsr == NULL)
        return NULL;

    struct cca_ast *ast = NULL;
    struct cca_ast *tmp = NULL;

    while (prsr->tkn->token_type != TK_EOF) {
        struct cca_ast *new = cca_pp_parse_expression(prsr, PR_LOW);
        if (new != NULL) {
            if (ast == NULL) {
                ast = new;
                tmp = ast;
            } else {
                tmp = ast;
                while (tmp->next != NULL)
                    tmp = tmp->next;
                tmp->next = new;
            }
        }

        cca_pp_next(prsr);
    }

    return ast;
}

enum cca_pp_precedence 
cca_pp_peek_precedence(struct cca_parser_pratt *prsr)
{
    if (prsr == NULL)
        return PR_LOW;

    switch (prsr->peek_tkn->token_type) {
        case TK_PLUS:
            return PR_SUM;
        case TK_MINUS:
            return PR_SUM;
        case TK_SLASH:
            return PR_PROD;
        case TK_ASTERISK:
            return PR_PROD;
        case TK_LPAR:
            return PR_PAR;
        default:
            return PR_LOW;
    }
}

bool
cca_pp_infix_has(enum cca_token_types tkn)
{
    size_t i = 0;
    for (i = 0; i < CCA_PP_INFIX_TABLE_SIZE; i++) {
        if (CCA_PP_INFIX_TABLE[i] == tkn)
            return true;
    }
    return false;
}

bool
cca_pp_prefix_has(enum cca_token_types tkn)
{
    size_t i = 0;
    for (i = 0; i < CCA_PP_PREFIX_TABLE_SIZE; i++) {
        if (CCA_PP_PREFIX_TABLE[i]  == tkn)
            return true;
    }

    return false;
}

void
cca_pp_free(struct cca_parser_pratt *prsr)
{
    if (prsr == NULL)
        return;

    free(prsr);
}
