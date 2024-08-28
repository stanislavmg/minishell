/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:01:04 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/28 13:33:01 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	string_is_spaces(const char *str)
{
	if (!str || !*str)
		return (1);
	while (ft_isspace(*str))
		str++;
	if (*str)
		return (0);
	return (1);
}

void	add_new_input(t_lexer *lex)
{
	char	*new_input;
	int		cur_pos;

	new_input = readline("> ");
	cur_pos = ft_strlen(lex->input);
	while (!*new_input || string_is_spaces(new_input))
	{
		lex->input = merge_str(lex->input, new_input);
		cur_pos = ft_strlen(lex->input);
		new_input = readline("> ");
	}
	lex->input = merge_str(lex->input, new_input);
	if (!lex->input)
		lex->err = ERR_MEM;
	lex->str_pos = lex->input + cur_pos;
}

size_t	ft_strncpy(char *d, const char *s, size_t n)
{
	size_t	i;

	i = 0;
	if (!d || !s || n == 0)
		return (0);
	while (i < n && s[i])
	{
		d[i] = s[i];
		i++;
	}
	d[i] = 0;
	return (i);
}

char	*merge_str(char *s1, char *s2)
{
	char	*res;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	if (!s1 && !s2)
		return (NULL);
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

char	*get_word(const char *s, size_t n)
{
	char	*new_word;

	if (!s)
		return (NULL);
	new_word = (char *)malloc(n + 1);
	if (!new_word)
		return (NULL);
	if (n == 0)
	{
		*new_word = 0;
		return (new_word);
	}
	ft_strncpy(new_word, s, n);
	return (new_word);
}
