#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "token.h"

struct cca_token* 
cca_token_default()
{
    struct cca_token* tkn = NULL;
    tkn = malloc(sizeof(struct cca_token));
    if (tkn == NULL)
        return NULL;

    // zero all the values
    memset(tkn, 0, sizeof(struct cca_token));
    tkn->token_type = TK_EOF;
    tkn->token_literal.symbol = 0;
    tkn->next = NULL;
    return tkn;
}

void
cca_token_free(struct cca_token *tkn)
{
    if (tkn == NULL)
        return;

    struct cca_token *tmp = tkn;

    while (tmp != NULL) {
        struct cca_token *next = tmp->next;
        free(tmp);
        tmp = next;
    }
}
