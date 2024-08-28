#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "parser_pratt.h"
#include "eval.h"

void repl()
{
    char buffer[512];
    while (true){
        memset(buffer, 0, sizeof(buffer));
        printf(">>> ");
        char *in = fgets(buffer, sizeof(buffer), stdin);
        if(strcmp(buffer, ";\n") == 0 || strcmp(buffer, "exit\n") == 0)
            break;

        if (in == NULL)
            break;

        struct cca_lexer *lxr = cca_lexer_new((void*)buffer, 512);
        if (lxr == NULL)
            return;

        struct cca_parser_pratt *prsr  = cca_pp_new(lxr);
        if (prsr == NULL) {
            if (lxr != NULL)
                free(lxr);

            return;
        }

        struct cca_ast *ast = cca_pp_parse(prsr);
        if (ast == NULL) {
            printf("error: unable to parse expression\n");
            return;
        }
        struct cca_ast *tmp = ast;

        while (tmp != NULL) {
            double result = cca_eval_ast(tmp);
            printf("%0.10lf\n", result);
            tmp = tmp->next;
        }


        if (ast != NULL)
            cca_ast_free(ast);

        if (prsr->_tokens != NULL)
            cca_token_free(prsr->_tokens);

        if (lxr != NULL)
            cca_lexer_free(lxr);

        if (prsr != NULL)
            cca_pp_free(prsr);
    }

}

int main(int argc, char** argv)
{
    repl();
}
