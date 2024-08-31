/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:30:58 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 16:52:29 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	is_var_delimeter(int ch)
{
	return (ch == '\\' || ch == '/' || ch == '=' || ch == '?'
		|| is_token_delimeter(ch) || is_catchar(ch));
}

static char	*new_var_name(t_lexer *lex)
{
	char	*var_name;
	int		i;

	i = 0;
	var_name = NULL;
	if (*lex->str_pos == '?')
	{
		var_name = ft_strdup("?");
		i = 1;
	}
	else
	{
		while (!is_var_delimeter(lex->str_pos[i]))
			i++;
		if (i == 0)
			var_name = get_word("$", 1);
		else
			var_name = get_word(lex->str_pos, i);
	}
	lex->str_pos += i;
	return (var_name);
}

char	*variable_handle(t_lexer *lex)
{
	char	*new_word;
	t_env	*var;

	var = NULL;
	lex->str_pos++;
	new_word = new_var_name(lex);
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
	else if (is_catchar(*lex->str_pos) || !ft_isspace(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}
