/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:30:46 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 17:13:14 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*string_handle(t_lexer *lex)
{
	char	*new_word;
	int		i;

	i = 0;
	new_word = NULL;
	if (!lex || !lex->str_pos)
		return (NULL);
	while (!is_token_delimeter(lex->str_pos[i]) && !is_catchar(lex->str_pos[i]))
		i++;
	new_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (ft_strchr(new_word, '*'))
	{
		expand_wildcard(lex, new_word);
		free(new_word);
		return (NULL);
	}
	if (!ft_isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}

char	*double_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;

	new_word = NULL;
	i = 0;
	if (!lex->in_qoutes)
		lex->str_pos++;
	lex->in_qoutes = 1;
	while (lex->str_pos[i] && lex->str_pos[i] != '\"' && lex->str_pos[i] != '$'
		&& lex->str_pos[i] != '\\')
		i++;
	new_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (*lex->str_pos == '$' || *lex->str_pos == '\\')
		new_word = merge_str(new_word, scan_token(lex));
	if (*lex->str_pos == '\"' && lex->in_qoutes)
	{
		lex->in_qoutes = 0;
		lex->str_pos++;
	}
	if (!ft_isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	if (lex->in_qoutes)
		lex->err = ERR_QUOTE;
	return (new_word);
}

char	*single_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = NULL;
	lex->in_qoutes = 1;
	lex->str_pos++;
	while (lex->str_pos[i] && lex->str_pos[i] != '\'')
		i++;
	if (lex->str_pos[i] != '\'')
	{
		lex->err = ERR_QUOTE;
		return (NULL);
	}
	new_word = get_word(lex->str_pos, i);
	lex->str_pos += i + 1;
	lex->in_qoutes = 0;
	if (!ft_isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}
