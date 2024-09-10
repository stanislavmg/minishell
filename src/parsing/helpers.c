/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:31:30 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 10:46:39 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_contain_alpha(const char *str)
{
	while (*str)
	{
		if (ft_isalpha(*str))
			return (1);
		str++;
	}
	return (0);
}

void	print_msh_err(char *token_name)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(token_name, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

t_token_type	get_token_type(t_parser *parser)
{
	if (!parser || !parser->cur_token_pos)
		return (0);
	return (((t_token *)parser->cur_token_pos->data)->type);
}
