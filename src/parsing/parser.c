#include "parser.h"

void here_doc_start(void)
{

}

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

int parse_redirect(t_token *token, t_exec_cmd *new_cmd)
{
	if (token->type == HERE_DOC)
		here_doc_start(); // TODO
	add_field_fname(token, new_cmd);
	add_field_open_mode(token->type, new_cmd);
	token->word = NULL;
	return (0);
}
	
void	push_var(t_list **var_lst, t_token *token)
{
	t_list	*new_lst_node;
	t_var	*new_var;

	new_var = new_tvar(token->word);
	token->word = NULL;
	new_lst_node = ft_lstnew(new_var);
	ft_lstadd_back(var_lst, new_lst_node);
}

t_cmd	*parse_cmd(t_parser *parser)
{
	t_exec_cmd	*new_cmd;
	t_list		*args;
	t_list		*var;
	t_token 	*cur_token;

	var = NULL;
	args = NULL;
	new_cmd = new_exec_cmd();
	if (!new_cmd)
	{
		parser->err = ERR_MEM;
		return (NULL);
	}
	while (parser->cur_token_pos && !is_cmd_delimeter(get_token_type(parser->cur_token_pos)))
	{
		cur_token = parser->cur_token_pos->data;
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			push_var(&var, cur_token);
		else if (is_redirect(cur_token->type))
			parse_redirect(cur_token, new_cmd);
		else if (cur_token->type == STRING)
		{
			parser->cmd_start = 1;
			ft_lstadd_back(&args, ft_lstnew(cur_token->word));
			cur_token->word = NULL;
		}
		parser->cur_token_pos = parser->cur_token_pos->next;
	}
	parser->cmd_start = 0;
	if (args)
		new_cmd->path = parsing_path(parser->paths, args->data);
	return (init_exec_cmd(var, args, new_cmd));
}

t_cmd	*init_exec_cmd(t_list *var, t_list *argv, t_exec_cmd *new_cmd)
{
	t_cmd	*var_assignment_tree;

	add_field_argv(argv, new_cmd);
	var_assignment_tree = build_var_tree(var);
	ft_lstclear(&argv, free);
	ft_lstclear(&var, free);
	if (var_assignment_tree && new_cmd)
		var_assignment_tree = add_tnode(var_assignment_tree, (t_cmd *)new_cmd, SEMICOLON);
	else if (!var_assignment_tree && new_cmd)
		var_assignment_tree = (t_cmd *)new_cmd;
	return (var_assignment_tree);
}

t_cmd	*build_var_tree(t_list *var)
{
	t_cmd		*var_assignment_tree;

	if (!var)
		return (NULL);
	var_assignment_tree = NULL;
	var_assignment_tree = var->data;
	var->data = NULL;
	var = var->next;
	while (var)
	{
		var_assignment_tree = add_tnode(var_assignment_tree, var->data, ASSIGNMENT);
		var->data = NULL;
		var = var->next;
	}
	return (var_assignment_tree);
}

int add_field_argv(t_list *args, t_exec_cmd *cmd)
{
	char	**argv;
	int		argc;
	int		i;

	if (!args || !cmd)
		return (1);
	i = -1;
	argc = ft_lstsize(args) + 1;
	argv = (char **)malloc(sizeof(char *) * argc);
	if (!argv)
		return (1);
	while (++i < argc - 1)
	{
		argv[i] = (char *)args->data;
		args->data = NULL;
		args = args->next;
	}
	argv[i] = NULL;
	cmd->argv = argv;
	return (0);
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

e_token get_token_type(t_list *token)
{
	return (((t_token *)token->data)->type);
}

t_cmd	*build_tree(t_parser *parser)
{
	t_cmd	*root;
	e_token	type;
	
	if (!parser->cur_token_pos)
		return (NULL);
	root = parse_cmd(parser);
	while (parser->cur_token_pos)
	{
		type = get_token_type(parser->cur_token_pos);
		parser->cur_token_pos = parser->cur_token_pos->next;
		if (type == OPEN_BRACKET)
		{
			parser->brackets_count++;
			root = add_tnode(root, build_tree(parser), type);
		}
		else if (type == CLOSED_BRACKET)
		{
			parser->brackets_count--;
			return (root);
		}
		else if (type == AND || type == OR || type == SEMICOLON)
			root = add_tnode(root, build_subtree(parser), type);
		else
			root = add_tnode(root,parse_cmd(parser), type);
	}
	return (root);
}

t_cmd	*build_subtree(t_parser *parser)
{
	e_token type;
	t_cmd	*root;
	
	root = NULL;
	type = get_token_type(parser->cur_token_pos);
	if (type == OPEN_BRACKET)
	{
		parser->cur_token_pos = parser->cur_token_pos->next;
		root = build_tree(parser);
	}
	else
		root = parse_cmd(parser);
	while (parser->cur_token_pos)
	{
		type = get_token_type(parser->cur_token_pos);
		if (type == AND || type == OR ||
			type == SEMICOLON || type == CLOSED_BRACKET)
			return (root);
		parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_tnode(root, parse_cmd(parser), type);
	}
	return (root);
}

t_cmd *build_AST(t_parser *parser)
{
	t_cmd		*root;

	root = build_tree(parser);
	if (!root)
		return (NULL);
	if (parser->brackets_count)
		printf("Brackets didn't close\n");
	return (root);
}
