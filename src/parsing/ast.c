# include "parser.h"

t_cmd *new_ast(t_parser *parser)
{
	t_cmd		*root;
	e_token		type;

	root = NULL;
	type = get_token_type(parser);
	if (is_cmd_delimeter(type) || type == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	else
		root = build_tree(parser);
	while (parser->cur_token_pos && !parser->err)
	{
		type = get_token_type(parser);
		if (is_redirect(type))
			type = REDIRECT;
		if (type == OPEN_BRACKET || type == CLOSED_BRACKET)
			parser->err = ERR_SYNTAX;
		else if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_tnode(root, build_tree(parser), type);
	}
	return (root);
}

t_cmd	*build_tree(t_parser *parser)
{
	t_cmd	*root;
	e_token	type;

	root = NULL;
	if (!parser || !parser->cur_token_pos)
		return (NULL);
	type = get_token_type(parser);
	if (is_cmd_delimeter(type))
		parser->err = ERR_SYNTAX;
	else if (type == OPEN_BRACKET)
	{
		parser->cur_token_pos = parser->cur_token_pos->next;
		root = parse_block(parser);
	}
	else
		root = parse_line(parser);
	return (root);
}

t_cmd	*parse_block(t_parser *parser)
{
	t_cmd *root;
	e_token	type;

	root = NULL;

	if (!parser || !parser->cur_token_pos || get_token_type(parser) == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	else
		root = build_tree(parser);
	type = get_token_type(parser);
	while (parser->cur_token_pos && CLOSED_BRACKET != type && !parser->err)
	{
		if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_tnode(root, build_tree(parser), type);
		type = get_token_type(parser);
	}
	if (CLOSED_BRACKET == type)
		parser->cur_token_pos = parser->cur_token_pos->next;
	else
		parser->err = ERR_SYNTAX;
	return (root);
}

void	print_msh_err(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",STDERR_FILENO);
	ft_putstr_fd(token->word, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

void remove_duplicate(t_list **redirects)
{
	t_redir	*r_token;
	t_list	*tmp;
	t_list	*prev;

	if (!redirects)
		return ;
	tmp = *redirects;
	prev = NULL;
	while (tmp)
	{
		r_token = tmp->data;
		if (HERE_DOC == r_token->type)
		{
			if (!prev)
				*redirects = tmp->next;
			else
				prev->next = tmp->next;
			unlink(r_token->fname);
			free(r_token->fname);
			ft_lstdelone(tmp, free);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	add_redirection_node(t_list **redir, t_parser *parser)
{
	t_cmd *new_redirect;
	t_token 	*cur_token;

	if (!parser || !parser->cur_token_pos)
		return ;
	cur_token = parser->cur_token_pos->data;
	if (cur_token->type == HERE_DOC)
		remove_duplicate(redir);
	new_redirect = parse_redirect(cur_token, parser->env_lst);
	ft_lstadd_front(redir, ft_lstnew(new_redirect));
}

void	add_cmd_arg(t_list **args, t_parser *parser)
{
	t_token 	*cur_token;

	if (!parser || !parser->cur_token_pos)
		return ;
	cur_token = parser->cur_token_pos->data;
	parser->cmd_start = 1;
	ft_lstadd_back(args, ft_lstnew(cur_token->word));
	cur_token->word = NULL;
}

t_cmd *new_cmd_tree(t_parser *parser)
{
	t_list		*args;
	t_list		*var;
	t_list		*redir;
	t_token 	*cur_token;

	var = NULL;
	args = NULL;
	redir = NULL;
	while (parser->cur_token_pos && !parser->err &&
		!is_cmd_delimeter(get_token_type(parser)) &&
		get_token_type(parser) != CLOSED_BRACKET)
	{
		cur_token = parser->cur_token_pos->data;
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			add_variable_node(&var, cur_token);
		else if (is_redirect(cur_token->type))
			add_redirection_node(&redir, parser);
		else if (cur_token->type == STRING)
			add_cmd_arg(&args, parser);
		if (cur_token->type == OPEN_BRACKET)
			parser->err = ERR_SYNTAX;
		else
			parser->cur_token_pos = parser->cur_token_pos->next;
	}
	parser->cmd_start = 0;
	return (parse_cmd(var, args, redir));
}

t_cmd *parse_line(t_parser *parser)
{
	t_cmd *root;

	root = NULL;
	if (is_cmd_delimeter(get_token_type(parser)) ||
		OPEN_BRACKET == get_token_type(parser))
	{
		parser->err = ERR_SYNTAX;
		return (NULL);
	}
	root = new_cmd_tree(parser);
	return (root);
}

t_cmd *parse_cmd(t_list *var, t_list *args, t_list *redir)
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

t_cmd	*build_tree_fromlist(t_list *nodes, e_token type)
{
	t_cmd	*root;

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