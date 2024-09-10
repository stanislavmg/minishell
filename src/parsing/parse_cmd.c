/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:31:43 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 11:00:01 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_cmd	*impl_new_cmd_tree(t_parser *parser,
									t_list *av,
									t_list *var,
									t_list *redir)
{
	t_token_type	type;
	t_token			*cur_token;

	type = get_token_type(parser);
	while (parser->cur_token_pos && !parser->err
		&& !is_cmd_delimeter(type) && type != CLOSED_BRACKET)
	{
		cur_token = parser->cur_token_pos->data;
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			push_variable(&var, cur_token);
		else if (is_redirect(cur_token->type))
			push_redirect(&redir, parser);
		else if (cur_token->type == ARG)
			push_arg(&av, parser);
		if (cur_token->type == OPEN_BRACKET)
			parser->err = ERR_SYNTAX;
		else if (parser->cur_token_pos && !parser->err)
			parser->cur_token_pos = parser->cur_token_pos->next;
		type = get_token_type(parser);
	}
	parser->cmd_start = 0;
	return (parse_cmd(var, av, redir));
}

t_cmd	*new_cmd_tree(t_parser *parser)
{
	return (impl_new_cmd_tree(parser, NULL, NULL, NULL));
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
	t_cmd			*new_redirect;
	char			*file_name;
	t_token			*cur_token;
	t_token_type	redir_mode;

	if (!parser || !parser->cur_token_pos)
		return ;
	cur_token = parser->cur_token_pos->data;
	redir_mode = cur_token->type;
	parser->cur_token_pos = parser->cur_token_pos->next;
	if (parser->cur_token_pos)
		cur_token = parser->cur_token_pos->data;
	else
		parser->err = ERR_SYNTAX;
	if (is_cmd_delimeter(cur_token->type) || is_redirect(cur_token->type)
		|| cur_token->type == OPEN_BRACKET || cur_token->type == CLOSED_BRACKET)
		parser->err = ERR_SYNTAX;
	if (parser->err)
		return ;
	file_name = cur_token->word;
	cur_token->word = NULL;
	if (redir_mode == HERE_DOC)
		remove_duplicate(redir);
	new_redirect = parse_redirect(redir_mode, file_name, parser->env_lst);
	ft_lstadd_front(redir, ft_lstnew(new_redirect));
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
