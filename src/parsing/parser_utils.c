/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:39:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 11:48:59 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
		root = add_ast_node(root, nodes->data, type);
		nodes->data = NULL;
		nodes = nodes->next;
	}
	return (root);
}

int	read_new_input(t_parser *parser)
{
	char	*input;
	t_list	*new_lst;

	if (!parser)
		return (1);
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
