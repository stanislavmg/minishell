/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:31:43 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 18:09:11 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_cmd	*new_cmd_tree(t_parser *parser)
{
	e_token	type;
	t_list	*args;
	t_list	*var;
	t_list	*redir;
	t_token	*cur_token;

	var = NULL;
	args = NULL;
	redir = NULL;
	type = get_token_type(parser);
	while (parser->cur_token_pos && !parser->err
		&& !is_cmd_delimeter(type) && type != CLOSED_BRACKET)
	{
		cur_token = parser->cur_token_pos->data;
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			push_variable(&var, cur_token);
		else if (is_redirect(cur_token->type))
			push_redirect(&redir, parser);
		else if (cur_token->type == STRING || cur_token->type == WILDCARD)
			push_arg(&args, parser);
		if (cur_token->type == OPEN_BRACKET)
			parser->err = ERR_SYNTAX;
		else if (parser->cur_token_pos && !parser->err)
			parser->cur_token_pos = parser->cur_token_pos->next;
		type = get_token_type(parser);
	}
	parser->cmd_start = 0;
	return (parse_cmd(var, args, redir));
}

void	push_variable(t_list **var_lst, t_token *token)
{
	t_list	*new_lst_node;
	t_var	*new_var;

	new_var = new_tvar(token->word);
	free(token->word);
	token->word = NULL;
	new_lst_node = ft_lstnew(new_var);
	ft_lstadd_back(var_lst, new_lst_node);
}

void	push_redirect(t_list **redir, t_parser *parser)
{
	t_cmd	*new_redirect;
	char	*file_name;
	t_token	*cur_token;
	e_token	redir_type;

	if (!parser || !parser->cur_token_pos)
		return ;
	cur_token = parser->cur_token_pos->data;
	redir_type = cur_token->type;
	parser->cur_token_pos = parser->cur_token_pos->next;
	if (parser->cur_token_pos)
		cur_token = parser->cur_token_pos->data;
	else
		parser->err = ERR_SYNTAX;
	if (is_cmd_delimeter(cur_token->type) || is_redirect(cur_token->type)
		|| cur_token->type == OPEN_BRACKET || cur_token->type == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	if (!parser->err)
	{
		file_name = cur_token->word;
		cur_token->word = NULL;
		if (redir_type == HERE_DOC)
			remove_duplicate(redir);
		new_redirect = parse_redirect(redir_type, file_name, parser->env_lst);
		ft_lstadd_front(redir, ft_lstnew(new_redirect));
	}
}

void	push_arg(t_list **args, t_parser *parser)
{
	t_token	*cur_token;
	t_list	*new_node;

	new_node = NULL;
	if (!parser || !parser->cur_token_pos)
		return ;
	cur_token = parser->cur_token_pos->data;
	parser->cmd_start = 1;
	new_node = ft_lstnew(cur_token->word);
	ft_lstadd_back(args, new_node);
	cur_token->word = NULL;
}

t_cmd	*parse_cmd(t_list *var, t_list *args, t_list *redir)
{
	t_cmd	*root;

	root = NULL;
	if (var)
	{
		root = build_tree_fromlist(var, ASSIGNMENT);
		ft_lstclear(&var, free);
	}
	if (args)
	{
		if (root)
			root = add_ast_node(root, (t_cmd *)new_exec_cmd(args), SEMICOLON);
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
