#include <stdio.h>
#include <stdlib.h>

#include "token.h"

#ifndef _CCA_LEXER_H
#define _CCA_LEXER_H

#define CCA_LEXER_END 0

#ifndef _CCA_TOKEN_H
#error
#endif

/// cca lexer
struct cca_lexer {
    void* stream;
    size_t size;
    char ch;
    char save_ch;
    size_t pos;
    size_t save_pos;
};

struct cca_lexer *cca_lexer_new(void*, size_t);
void cca_lexer_advance(struct cca_lexer*);
void cca_lexer_save(struct cca_lexer*);
void cca_lexer_restore(struct cca_lexer*);
void cca_lexer_skip_whitespace(struct cca_lexer*);
char cca_lexer_peek(struct cca_lexer*);

struct cca_token *cca_lexer_next(struct cca_lexer*);
struct cca_token *cca_lexer_parse_number(struct cca_lexer*);
struct cca_token *cca_lexer_lex(struct cca_lexer*);

void cca_lexer_free(struct cca_lexer*);

#endif
