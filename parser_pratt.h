#include "lexer.h"
#include "token.h"

#ifndef _CCA_PARSER_PRATT_H
#define _CCA_PARSER_PRATT_H

enum cca_parser_pratt_precedence {
    PR_LOW =        0,
    PR_SUM =        1,
    PR_PROD =       2,
    PR_PAR =        3,
};

struct cca_parser_pratt {
    struct cca_lexer *lxr;
    struct cca_token *tkn;
    struct cca_token *peek_tkn;
};

struct cca_ast *cca_pp_parse(struct cca_parser_pratt*);

#endif
