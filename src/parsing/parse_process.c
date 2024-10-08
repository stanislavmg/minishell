/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:46:24 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 10:58:04 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd	*new_ast(t_parser *parser)
{
	t_cmd			*root;
	t_token_type	type;

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
		if (type == OPEN_BRACKET || type == CLOSED_BRACKET || type == ARG)
			parser->err = ERR_SYNTAX;
		if (is_cmd_delimeter(type))
			parser->cur_token_pos = parser->cur_token_pos->next;
		root = add_ast_node(root, build_tree(parser), type);
	}
	return (root);
}

t_cmd	*build_tree(t_parser *parser)
{
	t_cmd			*root;
	t_token_type	type;

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
	t_cmd			*root;
	t_token_type	type;

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
	if (CLOSED_BRACKET == type && !parser->err)
	{
		parser->cur_token_pos = parser->cur_token_pos->next;
		parser->brackets_count--;
	}
	else
		parser->err = ERR_SYNTAX;
	return (root);
}

t_cmd	*parse_line(t_parser *parser)
{
	t_cmd	*root;

	if (!parser)
		return (NULL);
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

t_cmd	*parse_redirect(t_token_type redir, char *fname, t_list *env)
{
	t_redir	*new_node;

	new_node = NULL;
	if (!fname || !env)
		return (NULL);
	if (redir == HERE_DOC || redir == EXPAND_HERE_DOC)
		fname = here_doc_start(fname, env, redir);
	if (fname)
		new_node = new_redir(redir, fname);
	return ((t_cmd *)new_node);
}
