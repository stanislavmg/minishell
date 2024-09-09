/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:30:58 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 20:59:11 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_var_delimeter(int ch)
{
	return (ch == 0 || ch == '\\' || ch == '/' || ch == '=' || ch == '?'
		|| is_token_delimeter(ch) || is_catchar(ch));
}

char	*new_var_name(char *str)
{
	char	*var_name;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	var_name = NULL;
	if (*str == '?')
		var_name = ft_strdup("?");
	else
	{
		while (!is_var_delimeter(str[i]))
			i++;
		if (i == 0)
			var_name = get_word("$", 1);
		else
			var_name = get_word(str, i);
	}
	return (var_name);
}

char	*variable_handle(t_lexer *lex)
{
	char	*new_word;
	t_env	*var;

	var = NULL;
	lex->str_pos++;
	new_word = new_var_name(lex->str_pos);
	if (*lex->str_pos && *new_word == *lex->str_pos)
		lex->str_pos += ft_strlen(new_word);
	if (ft_strcmp(new_word, "$"))
	{
		var = get_env(lex->env, new_word);
		free(new_word);
		if (var)
			new_word = ft_strdup(var->value);
		else
			new_word = NULL;
	}
	if (lex->in_qoutes)
		new_word = merge_str(new_word, double_quotes_handle(lex));
	else if (!ft_isspace(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}
