/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_slash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:30:52 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 20:55:01 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_screening_ch(int ch)
{
	return (ch == '`' || ch == '\"' || ch == '$' || ch == '\\');
}

static char	*get_str(t_lexer *lex)
{
	char	*res;
	char	del;
	int		i;

	i = 0;
	res = NULL;
	del = *lex->str_pos;
	lex->str_pos++;
	while (lex->str_pos[i] && lex->str_pos[i] != del)
		i++;
	if (lex->str_pos[i] != del)
	{
		lex->err = ERR_QUOTE;
		return (NULL);
	}
	if (i)
		res = get_word(lex->str_pos, i);
	lex->str_pos += i;
	lex->str_pos++;
	return (res);
}

char	*get_hd_stop_word(t_lexer *lex, char *stop)
{
	int		i;

	i = 0;
	while (ft_isspace(*lex->str_pos))
		lex->str_pos++;
	if (*lex->str_pos == '\'' || *lex->str_pos == '\"')
		stop = get_str(lex);
	else
	{
		while (!is_token_delimeter(lex->str_pos[i])
			&& lex->str_pos[i] != '\'' && lex->str_pos[i] != '\"')
			i++;
		if (i)
			stop = get_word(lex->str_pos, i);
		lex->str_pos += i;
	}
	if (lex->err)
		return (NULL);
	if (*lex->str_pos && !ft_isspace(*lex->str_pos)
		&& !is_metachar(*lex->str_pos))
		stop = merge_str(stop, get_hd_stop_word(lex, NULL));
	return (stop);
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
