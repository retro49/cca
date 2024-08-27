#include <stdbool.h>

#include "lexer.h"
#include "token.h"

#ifndef _CCA_PARSER_PRATT_H
#define _CCA_PARSER_PRATT_H

enum cca_pp_precedence {
    PR_LOW =        0,
    PR_SUM =        1,
    PR_PROD =       2,
    PR_PREFIX =     3,
    PR_PAR =        4,
};


#define CCA_PP_PREFIX_TABLE_SIZE      3
#define CCA_PP_INFIX_TABLE_SIZE       4


bool cca_pp_infix_has(enum cca_token_types);
bool cca_pp_prefix_has(enum cca_token_types);

struct cca_parser_pratt {
    struct cca_lexer *lxr;
    struct cca_token *_tokens;
    struct cca_token *_tokens_tmp;
    struct cca_token *tkn;
    struct cca_token *peek_tkn;
};

struct cca_parser_pratt* cca_pp_new(struct cca_lexer*);
struct cca_ast* cca_pp_parse_expression(struct cca_parser_pratt*, enum cca_pp_precedence);
enum cca_pp_precedence cca_pp_peek_precedence(struct cca_parser_pratt*);
void cca_pp_next(struct cca_parser_pratt*);

struct cca_ast *cca_pp_parse(struct cca_parser_pratt*);

#endif
