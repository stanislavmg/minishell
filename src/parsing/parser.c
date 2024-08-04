#include "parser.h"

char *here_doc_start(void)
{
	return (NULL);
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

t_cmd *parse_redirect(t_token *token)
{
	t_redir *new_node;

	if (token->type == HERE_DOC)
		new_node = new_redir(token->type, here_doc_start());
	else
	{
		new_node = new_redir(token->type, token->word);
		token->word = NULL;
	}
	return ((t_cmd *)new_node);
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

t_cmd *new_branch(t_list *var, t_list *args, t_list *redir)
{
	t_cmd *root;

	root = NULL;
	if (var)
	{
		root = build_tree_fromlist(var, ASSIGNMENT);
		ft_lstclear(&var, free);
	}
	if (args)
	{
		if (root)
			root = add_tnode(root, (t_cmd *)new_exec_cmd(args), SEMICOLON);
		else
			root = (t_cmd *)new_exec_cmd(args);
		ft_lstclear(&args, free);
	}
	if (redir)
	{
		if (root)
			ft_lstadd_front(&redir, ft_lstnew(root));
		root = build_tree_fromlist(redir, REDIRECT);
		ft_lstclear(&redir, free);
	}
	return (root);
}

t_cmd	*parse_cmd(t_parser *parser)
{
	t_list		*args;
	t_list		*var;
	t_list		*redir;
	t_token 	*cur_token;

	var = NULL;
	args = NULL;
	redir = NULL;
	while (parser->cur_token_pos && !is_cmd_delimeter(get_token_type(parser->cur_token_pos)))
	{
		cur_token = parser->cur_token_pos->data;
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			push_var(&var, cur_token);
		else if (is_redirect(cur_token->type))
			ft_lstadd_back(&redir, ft_lstnew(parse_redirect(cur_token)));
		else if (cur_token->type == STRING)
		{
			parser->cmd_start = 1;
			ft_lstadd_back(&args, ft_lstnew(cur_token->word));
			cur_token->word = NULL;
		}
		parser->cur_token_pos = parser->cur_token_pos->next;
	}
	parser->cmd_start = 0;
	return (new_branch(var, args, redir));
}

t_cmd	*build_tree_fromlist(t_list *nodes, e_token type)
{
	t_cmd		*root;

	if (!nodes)
		return (NULL);
	root = NULL;
	root = nodes->data;
	nodes->data = NULL;
	nodes = nodes->next;
	while (nodes)
	{
		root = add_tnode(root, nodes->data, type);
		nodes->data = NULL;
		nodes = nodes->next;
	}
	return (root);
}

char **add_field_argv(t_list *args)
{
	char	**argv;
	int		argc;
	int		i;

	if (!args)
		return (NULL);
	i = -1;
	argc = ft_lstsize(args) + 1;
	argv = (char **)malloc(sizeof(char *) * argc);
	if (!argv)
		return (NULL);
	while (++i < argc - 1)
	{
		argv[i] = (char *)args->data;
		args->data = NULL;
		args = args->next;
	}
	argv[i] = NULL;
	return (argv);
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
	root = NULL;
	root = parse_cmd(parser);
	while (parser->cur_token_pos)
	{
		type = get_token_type(parser->cur_token_pos);
		if (is_redirect(type))
			type = REDIRECT;
		else
			parser->cur_token_pos = parser->cur_token_pos->next;
		if (type == OPEN_BRACKET)
		{
			parser->brackets_count++;
			if (!root)
				root = build_tree(parser);
			else
				root = add_tnode(root, build_tree(parser), type);
		}
		else if (type == CLOSED_BRACKET)
		{
			parser->brackets_count--;
			return (root);
		}
		else if (type == AND || type == OR || type == SEMICOLON || type == PIPE)
			root = add_tnode(root, build_subtree(parser), type);
		else
			root = add_tnode(root, parse_cmd(parser), type);
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
	// if (parser->brackets_count)
	// 	printf("Brackets didn't close\n");
	return (root);
}
