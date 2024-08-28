#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "lexer.h"
#include "retest.h"

#ifdef _RETEST_H
#define CCA_TEST_INIT
RETEST_TESTING_INIT

void 
cca_lexer_test()
{

    struct cca_lexer* lxr = NULL;
    const char* stream = "1 + thevar;";

    lxr = cca_lexer_new((void*)stream, strlen(stream));

    if (lxr == NULL)
        return;

    RETEST_ASSERT_EQ(lxr->pos, 0);
    RETEST_ASSERT(lxr->ch == '1');
    RETEST_ASSERT_EQ((cca_lexer_peek(lxr)), ' ');
    size_t i = 0;
    for (i = 0; i < lxr->size - 1; i++) {
        cca_lexer_advance(lxr);
    }
    RETEST_ASSERT_EQ(lxr->pos, (lxr->size - 1));
    RETEST_ASSERT_EQ(lxr->ch, ';');
    RETEST_ASSERT_EQ((cca_lexer_peek(lxr)), 0);
    cca_lexer_advance(lxr);
    RETEST_ASSERT_EQ(lxr->ch, 0);
    RETEST_ASSERT_EQ(lxr->pos, lxr->size);
    RETEST_ASSERT_EQ((cca_lexer_peek(lxr)), 0);
}

void
cca_lexer_lex_test()
{

    struct cca_lexer* lxr = NULL;
    const char* stream = "1.123 + 123 * 33 () ;";

    lxr = cca_lexer_new((void*)stream, strlen(stream));
    struct cca_token *tkns = cca_lexer_lex(lxr);
    struct cca_token *tmp = tkns;
    if (tkns == NULL)
        RETEST_ASSERT(false);

    RETEST_ASSERT(tmp->token_type == TK_NUMBER);
    RETEST_ASSERT((strcmp("1.123", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_PLUS);
    RETEST_ASSERT(tmp->token_literal.symbol == '+');
    
    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_NUMBER);
    RETEST_ASSERT((strcmp("123", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_ASTERISK);
    RETEST_ASSERT(tmp->token_literal.symbol == '*');

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_NUMBER);
    RETEST_ASSERT((strcmp("33", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_LPAR);
    RETEST_ASSERT((strcmp("(", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_RPAR);
    RETEST_ASSERT((strcmp(")", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_SEMICOLON);
    RETEST_ASSERT((strcmp(";", tmp->token_literal.literal) == 0));

    tmp = tmp->next;
    assert(tmp != NULL);
    RETEST_ASSERT(tmp->token_type == TK_EOF);
    RETEST_ASSERT(tmp->token_literal.symbol == 0);

    tmp = tmp->next;
    assert(tmp == NULL);

    if (tkns != NULL)
        cca_token_free(tkns);
}

RETEST_TESTING_START
RETEST_TESTING_ADD(cca_lexer_test)
RETEST_TESTING_ADD(cca_lexer_lex_test)
RETEST_TESTING_END
#else
#error
#endif
