#include "parser.h"

t_var	*new_tvar(const char *key_and_value)
{
	t_var	*new_var;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		return (NULL);
	new_var->type = VARIABLE;
	new_var->key = get_var_name(key_and_value);
	new_var->value = get_var_value(key_and_value);
	return (new_var);
}

t_cmd *parse_redirect(t_token *token, t_list *env)
{
	t_redir *new_node;

	if (token->type == HERE_DOC)
		here_doc_start(token, env);
	new_node = new_redir(token->type, token->word);
	token->word = NULL;
	return ((t_cmd *)new_node);
}

void	add_variable_node(t_list **var_lst, t_token *token)
{
	t_list	*new_lst_node;
	t_var	*new_var;

	new_var = new_tvar(token->word);
	token->word = NULL;
	new_lst_node = ft_lstnew(new_var);
	ft_lstadd_back(var_lst, new_lst_node);
}

t_cmd	*add_tnode(t_cmd *left_node, t_cmd *right_node, int type)
{
	t_ast	*root;

	root = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!root)
		return (NULL);
	root->left = left_node;
	root->right = right_node;
	root->type = type;
	return ((t_cmd *)root);
}

e_token get_token_type(t_parser *parser)
{
	if (!parser || !parser->cur_token_pos)
		return (0);
	return (((t_token *)parser->cur_token_pos->data)->type);
}
