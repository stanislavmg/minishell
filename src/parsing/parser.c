/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:40:46 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/11 11:40:20 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_redir	*new_redir(t_token_type redirect_type, char *fname)
{
	t_redir	*new_node;

	new_node = (t_redir *)malloc(sizeof(t_redir));
	new_node->type = redirect_type;
	new_node->fname = fname;
	if (OUTPUT_TRUNC == redirect_type)
		new_node->mode = (O_TRUNC | O_CREAT | O_RDWR);
	else if (OUTPUT_ADD == redirect_type)
		new_node->mode = (O_APPEND | O_CREAT | O_RDWR);
	else if (INPUT_TRUNC == redirect_type)
		new_node->mode = O_RDONLY;
	else
		new_node->mode = 0;
	return (new_node);
}

t_parser	*new_parser(t_list *tokens, t_list *env)
{
	t_parser	*parser;

	parser = (t_parser *)ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->cur_token_pos = tokens;
	parser->env_lst = env;
	return (parser);
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

t_cmd	*add_ast_node(t_cmd *left_node, t_cmd *right_node, int type)
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
