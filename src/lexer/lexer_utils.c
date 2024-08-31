/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:00:03 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 00:30:01 by sgoremyk         ###   ########.fr       */
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
		|| OUTPUT_ADD == type || OUTPUT_TRUNC == type);
}

int	is_metachar(char ch)
{
	return ('|' == ch || '&' == ch || ';' == ch
		|| '<' == ch || '>' == ch || '(' == ch || ')' == ch);
}

int	is_redirectchar(char ch)
{
	return ('<' == ch || '>' == ch);
}

int	is_catchar(char ch)
{
	return ('$' == ch || '\"' == ch || '\'' == ch || '\\' == ch);
}

int	is_token_delimeter(int ch)
{
	return (ch == 0 || ft_isspace(ch)
		|| is_metachar(ch) || is_redirectchar(ch));
}

int	is_screening_ch(int ch)
{
	return (ch == '`' || ch == '\"' || ch == '$' || ch == '\\');
}

int	is_var_delimeter(int ch)
{
	return (ch == '\\' || ch == '/' || ch == '=' || ch == '?');
}
