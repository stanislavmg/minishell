/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:46:24 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 11:17:27 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	read_new_input(t_parser *parser)
{
	char	*input;
	t_list	*new_lst;

	input = readline("> ");
	if (!input)
		return (1);
	new_lst = new_token_list(parser->env_lst, input);
	if (!new_lst)
	{
		free(input);
		return (1);
	}
	ft_lstadd_back(&parser->cur_token_pos, new_lst);
	return (0);
}

t_cmd	*new_ast(t_parser *parser)
{
	t_cmd		*root;
	e_token		type;

	root = NULL;
	if (!parser)
		return (NULL);
	type = get_token_type(parser);
	if (is_cmd_delimeter(type) || type == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	else
		root = build_tree(parser);
	while (parser->cur_token_pos && !parser->err)
	{
		type = get_token_type(parser);
		if (type == PIPE && !parser->cur_token_pos->next)
			read_new_input(parser);
		if (is_redirect(type))
			type = REDIRECT;
		if (type == OPEN_BRACKET || type == CLOSED_BRACKET)
			parser->err = ERR_SYNTAX;
		else if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_ast_node(root, build_tree(parser), type);
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
		parser->brackets_count++;
		if (parser->brackets_count > 255)
			parser->err = ERR_BRACKETS;
		parser->cur_token_pos = parser->cur_token_pos->next;
		root = parse_block(parser);
	}
	else
		root = parse_line(parser);
	return (root);
}

t_cmd	*parse_block(t_parser *parser)
{
	t_cmd	*root;
	e_token	type;

	root = NULL;
	if (!parser || !parser->cur_token_pos
		|| get_token_type(parser) == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	else
		root = build_tree(parser);
	type = get_token_type(parser);
	while (parser->cur_token_pos && CLOSED_BRACKET != type && !parser->err)
	{
		if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_ast_node(root, build_tree(parser), type);
		type = get_token_type(parser);
	}
	if (CLOSED_BRACKET == type)
		parser->cur_token_pos = parser->cur_token_pos->next;
	else
		parser->err = ERR_SYNTAX;
	return (root);
}

t_cmd	*parse_line(t_parser *parser)
{
	t_cmd	*root;

	root = NULL;
	if (is_cmd_delimeter(get_token_type(parser))
		|| OPEN_BRACKET == get_token_type(parser))
	{
		parser->err = ERR_SYNTAX;
		return (NULL);
	}
	root = new_cmd_tree(parser);
	return (root);
}

t_cmd	*parse_redirect(e_token redir_type, char *fname, t_list *env)
{
	t_redir	*new_node;

	new_node = NULL;
	if (redir_type == HERE_DOC)
		fname = here_doc_start(fname, env);
	if (fname)
		new_node = new_redir(redir_type, fname);
	return ((t_cmd *)new_node);
}
