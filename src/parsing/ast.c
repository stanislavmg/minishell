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
		else if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		if (type == OPEN_BRACKET || type == CLOSED_BRACKET)
			parser->err = ERR_SYNTAX;
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
	else if (type == CLOSED_BRACKET)
		return (root);
	else if (type == OPEN_BRACKET)
	{
		parser->cur_token_pos = parser->cur_token_pos->next;
		root = parse_block(parser);
	}
	else
		root = parse_cmd(parser);
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
		if (is_redirect(type))
			type = REDIRECT;
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
	printf("minishell: syntax error near unexpected token `%s'\n", token->word);
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
	if (is_cmd_delimeter(get_token_type(parser)) || OPEN_BRACKET == get_token_type(parser))
	{
		parser->err = ERR_SYNTAX;
		return (NULL);
	}
	while (parser->cur_token_pos && 
			!parser->err &&

			!is_cmd_delimeter(get_token_type(parser)) &&
			get_token_type(parser) != CLOSED_BRACKET)
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
		if (cur_token->type == OPEN_BRACKET)
			parser->err = ERR_SYNTAX;
		else
			parser->cur_token_pos = parser->cur_token_pos->next;
	}
	parser->cmd_start = 0;
	return (new_branch(var, args, redir));
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