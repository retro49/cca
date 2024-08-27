#include "ast.h"
#include "eval.h"

double cca_eval_ast_prefix(struct cca_ast *ast)
{
    switch (ast->ast.prefix->symbol) {
        case TK_MINUS:
            {
                double right = cca_eval_ast(ast->ast.prefix->right);
                if (ast->ast.prefix->symbol == '-')
                    right = right * -1;
                return right;
            }
            break;
        default:
            return 0;
    }
}

double cca_eval_ast_infix(struct cca_ast *ast)
{
    double left = 0;
    double right = 0;
    left = cca_eval_ast(ast->ast.infix->left);
    right = cca_eval_ast(ast->ast.infix->right);
    switch (ast->ast.infix->symbol) {
        case TK_PLUS:
            return left + right;
        case TK_MINUS:
            return left - right;
        case TK_ASTERISK:
            return left * right;
        case TK_SLASH:
            if (right == 0)  {
                printf("zero division error");
                return 0;
            }
            return left / right;
            
    }

    return 0;
}

double cca_eval_ast(struct cca_ast *ast)
{
    if (ast  == NULL)
        return 0;

    switch (ast->ast_type) {
        case AST_NUMBER: 
            return ast->ast.number->number;
        case AST_PREFIX:
            return cca_eval_ast_prefix(ast);
        case AST_INFIX:
            return cca_eval_ast_infix(ast);
        default:
            return 0;
    }
}
