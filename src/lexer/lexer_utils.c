/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:00:03 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 16:19:13 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_cmd_delimeter(e_token type)
{
	return (AND == type || PIPE == type
		|| OR == type || SEMICOLON == type);
}

int	is_redirect(e_token type)
{
	return (HERE_DOC == type || INPUT_TRUNC == type
		|| OUTPUT_ADD == type || OUTPUT_TRUNC == type
		|| EXPAND_HERE_DOC == type);
}

int	is_metachar(char ch)
{
	return ('|' == ch || '&' == ch || ';' == ch
		|| '<' == ch || '>' == ch || '(' == ch || ')' == ch);
}

int	is_catchar(char ch)
{
	return ('$' == ch || '\"' == ch || '\'' == ch || '\\' == ch);
}

int	is_token_delimeter(int ch)
{
	return (ch == 0 || ft_isspace(ch) || is_metachar(ch));
}
