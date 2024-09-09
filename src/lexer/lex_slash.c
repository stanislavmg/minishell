/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_slash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:30:52 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 20:59:45 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_screening_ch(int ch)
{
	return (ch == '`' || ch == '\"' || ch == '$' || ch == '\\');
}

// cat << ''"\ #
char	*get_hd_stop_word(t_lexer *lex, char *stop_word)
{
	int		i;
	char	qoutes;

	i = 0;
	qoutes = 0;
	while (ft_isspace(*lex->str_pos))
		lex->str_pos++;
	if (*lex->str_pos == '\'' || *lex->str_pos == '\"')
	{
		qoutes = *lex->str_pos;
		lex->str_pos++;
	}
	while (!is_token_delimeter(lex->str_pos[i]) && lex->str_pos[i] != qoutes)
		i++;
	if (qoutes && lex->str_pos[i] != qoutes)
		lex->err = ERR_SYNTAX;
	if (i)
		stop_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (qoutes)
		lex->str_pos++;
	if (*lex->str_pos && !ft_isspace(*lex->str_pos)
		&& !is_metachar(*lex->str_pos))
		stop_word = merge_str(stop_word, get_hd_stop_word(lex, NULL));
	return (stop_word);
}

char	*slash_handle(t_lexer *lex)
{
	char	*new_word;

	lex->str_pos++;
	if (lex->in_qoutes)
	{
		if (is_screening_ch(*lex->str_pos))
		{
			new_word = get_word(lex->str_pos, 1);
			lex->str_pos++;
		}
		else
			new_word = ft_strdup("\\");
		new_word = merge_str(new_word, double_quotes_handle(lex));
	}
	else if (!*lex->str_pos)
		new_word = get_word("\\", 1);
	else
	{
		new_word = get_word(lex->str_pos, 1);
		if (*lex->str_pos)
			lex->str_pos++;
	}
	if (!ft_isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}
