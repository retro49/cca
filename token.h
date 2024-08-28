#ifndef _CCA_TOKEN_H
#define _CCA_TOKEN_H

#define CCA_MAX_TOKEN_BUFFER 512 /// too much for token ?

/// token types that are in cca
enum cca_token_types {
    TK_EOF = 0,
    TK_INVALID = 1,
    TK_PLUS = '+',
    TK_MINUS = '-',
    TK_ASTERISK = '*',
    TK_SLASH = '/',

    TK_CARET = '^',
    TK_MOD = '%',

    TK_LPAR = '(',
    TK_RPAR = ')',
    TK_SEMICOLON = ';',

    // for tokens that cannot be represented
    // by a single byte, are added below TK_NUMBER
    TK_NUMBER = 256,
};

struct cca_token {
    enum cca_token_types token_type;
    double number;
    union {
        char literal[CCA_MAX_TOKEN_BUFFER];
        char symbol;
    }token_literal;

    struct cca_token *next;
};

struct cca_token* cca_token_default();
void cca_token_free(struct cca_token*);

#endif
