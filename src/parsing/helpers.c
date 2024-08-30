/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:31:30 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 17:40:01 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_msh_err(char *token_name)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(token_name, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

e_token	get_token_type(t_parser *parser)
{
	if (!parser || !parser->cur_token_pos)
		return (0);
	return (((t_token *)parser->cur_token_pos->data)->type);
}
