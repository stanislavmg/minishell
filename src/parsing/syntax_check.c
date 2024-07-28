#include "parser.h"

t_parser *new_parser(t_lexer *lex)
{
	t_parser *pars_info;

	pars_info = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!pars_info)
		return (NULL);
	pars_info->token = lex->tokens;
	return (pars_info);
}

int exec_pipe(t_ast *root)
{
	if (root->left->type == PIPE)
		exec_pipe
}

int	travers_tree(t_ast *root)
{
	if (!root)
		return (0);
	if (root->type == PIPE)
		exec_pipe(root); // TODO
	else if (root->type == ASSIGNMENT)
		add_var(root); // TODO
	else if (root->type == OR)
	{
		if (!travers_tree(root->left) && get_last_status()) // TODO
			travers_tree(root->right);
	}
	else if (root->type == AND)
	{
		if (!travers_tree(root->left) && !get_last_status()) // TODO
			travers_tree(root->right);
	}
	else
	{
		travers_tree(root->left);
		travers_tree(root->right);
	}

}