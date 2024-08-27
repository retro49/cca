#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stdbool.h>

#include "lexer.h"
#include "util.h"

struct cca_lexer* 
cca_lexer_new(void *stream, size_t size)
{
    if (stream == NULL)
        return NULL;

    if (size <= 0)
        return NULL;

    struct cca_lexer *lxr = NULL;
    lxr = malloc(sizeof(struct cca_lexer));
    if (lxr == NULL)
        return NULL;

    lxr->stream = stream;
    lxr->size = size;
    lxr->pos = 0;
    lxr->ch = *(char*)stream;
}

void
cca_lexer_advance(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return;

    if (lxr->pos >= lxr->size) {
        lxr->ch = CCA_LEXER_END;
        return;
    }

    lxr->pos += 1;
    lxr->ch = *(char*)(lxr->stream + (lxr->pos));
}

void
cca_lexer_save(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return;

    lxr->save_pos = lxr->pos;
    lxr->save_ch = lxr->ch;
}

void
cca_lexer_restore(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return;
    lxr->pos = lxr->save_pos;
    lxr->ch = lxr->save_ch;
}

void 
cca_lexer_skip_whitespace(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return;

    while (cca_util_is_whitespace(lxr->ch))
        cca_lexer_advance(lxr);
}

char
cca_lexer_peek(struct cca_lexer *lxr)
{
    if (lxr->pos >= lxr->size - 1)
        return CCA_LEXER_END;
    return *((char*)lxr->stream + (lxr->pos + 1));
}

void 
cca_lexer_free(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return;

    free(lxr);
}

struct cca_token* 
cca_lexer_next(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return NULL;

    cca_lexer_skip_whitespace(lxr);
    struct cca_token *tkn = NULL;
    switch (lxr->ch) {
        case 0:
            tkn = cca_token_default();
            break;
        case '+':
            tkn = cca_token_default();
            tkn->token_literal.symbol = '+';
            tkn->token_type = TK_PLUS;
            cca_lexer_advance(lxr);
            break;
        case '-':
            tkn = cca_token_default();
            tkn->token_literal.symbol = '-';
            tkn->token_type = TK_MINUS;
            cca_lexer_advance(lxr);
            break;
        case'*':
            tkn = cca_token_default();
            tkn->token_literal.symbol = '*';
            tkn->token_type = TK_ASTERISK;
            cca_lexer_advance(lxr);
            break;
        case '/':
            tkn = cca_token_default();
            tkn->token_literal.symbol = '/';
            tkn->token_type = TK_SLASH;
            cca_lexer_advance(lxr);
            break;
        case '(':
            tkn = cca_token_default();
            tkn->token_literal.symbol = '(';
            tkn->token_type = TK_LPAR;
            cca_lexer_advance(lxr);
            break;
        case ')':
            tkn = cca_token_default();
            tkn->token_literal.symbol = ')';
            tkn->token_type = TK_RPAR;
            cca_lexer_advance(lxr);
            break;
        case ';':
            tkn = cca_token_default();
            tkn->token_literal.symbol = ';';
            tkn->token_type = TK_SEMICOLON;
            cca_lexer_advance(lxr);
            break;
        default:
            if (cca_util_is_numeric(lxr->ch)) {
                tkn = cca_lexer_parse_number(lxr);
            } else {
                tkn = cca_token_default();
                tkn->token_type = TK_INVALID;
            }
    }

    return tkn;
}

struct cca_token*
cca_lexer_parse_number(struct cca_lexer *lxr)
{
    if (lxr == NULL)
        return NULL;

    struct cca_token* tkn = cca_token_default();
    if (tkn == NULL)
        return NULL;

    tkn->token_type = TK_NUMBER;
    memset(tkn->token_literal.literal, 0, CCA_MAX_TOKEN_BUFFER);

    uint8_t dot_seen = 0;
    size_t iter = 0;
    while (dot_seen < 2 && 
            iter < CCA_MAX_TOKEN_BUFFER && 
            (cca_util_is_numeric(lxr->ch) || lxr->ch == '.')) {

        if (lxr->ch == '.')
            dot_seen += 1;

        *(tkn->token_literal.literal + iter) = lxr->ch;
        cca_lexer_advance(lxr);
        iter += 1;
    }
    tkn->number = atof(tkn->token_literal.literal);
    return tkn;
}

struct cca_token*
cca_lexer_lex(struct cca_lexer *lxr)
{
    struct cca_token *head = NULL;
    struct cca_token *tmp = NULL;
    struct cca_token *tkn = NULL;

    while (true) {
        tkn = cca_lexer_next(lxr);

        if (head == NULL) {
            if (tkn == NULL)
                break;
            head = tkn;
            tmp = head;
        } else {
            tmp = head;
            while (tmp->next != NULL)
                tmp = tmp->next;
            if (tkn == NULL) {
                tmp->next = NULL;
                break;
            } else {
                tmp->next = tkn;
            }
        }

        if (tkn->token_type == TK_EOF)
            break;
    }

    return head;
}

void
cca_lexer_free_lex(struct cca_token *tkns)
{
    if (tkns == NULL)
        return;

    cca_token_free(tkns);
}
