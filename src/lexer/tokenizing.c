/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:20:26 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 00:47:57 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	start_tokenization(t_lexer *lex)
{
	char	*new_word;

	new_word = NULL;
	while (*lex->str_pos && !lex->err)
	{
		new_word = scan_token(lex);
		if (new_word)
			push_token(&lex->tokens, new_word, STRING);
	}
	if (!string_is_spaces(lex->input))
		add_history(lex->input);
	free(lex->input);
}

char	*scan_token(t_lexer *lex)
{
	char	*new_word;

	new_word = NULL;
	if (lex->err)
		return (NULL);
	if (!lex || !*lex->str_pos)
		return (NULL);
	while (ft_isspace(*lex->str_pos))
		lex->str_pos++;
	if (!*lex->str_pos)
		return (NULL);
	if (!strncmp(lex->str_pos, "\"", 1))
		new_word = double_quotes_handle(lex);
	else if (is_metachar(*lex->str_pos))
		metachar_handle(lex);
	else if (!strncmp(lex->str_pos, "\'", 1))
		new_word = single_quotes_handle(lex);
	else if (!strncmp(lex->str_pos, "\\", 1))
		new_word = slash_handle(lex);
	else if (!strncmp(lex->str_pos, "$", 1))
		new_word = variable_handle(lex);
	else
		new_word = string_handle(lex);
	return (new_word);
}

char	*redirect_handle(t_lexer *lex)
{
	if (!strncmp(lex->str_pos, "<<", 2))
		default_handle(lex, "<<", HERE_DOC);
	else if (!strncmp(lex->str_pos, "<", 1))
		default_handle(lex, "<", INPUT_TRUNC);
	else if (!strncmp(lex->str_pos, ">>", 2))
		default_handle(lex, ">>", OUTPUT_ADD);
	else if (!strncmp(lex->str_pos, ">", 1))
		default_handle(lex, ">", OUTPUT_TRUNC);
	return (NULL);
}

char	*metachar_handle(t_lexer *lex)
{
	if (is_redirectchar(*lex->str_pos))
		redirect_handle(lex);
	else if (!strncmp(lex->str_pos, "||", 2))
		default_handle(lex, "||", OR);
	else if (!strncmp(lex->str_pos, "|", 1))
		default_handle(lex, "|", PIPE);
	else if (!strncmp(lex->str_pos, "&&", 2))
		default_handle(lex, "&&", AND);
	else if (!strncmp(lex->str_pos, ";", 1))
		default_handle(lex, ";", SEMICOLON);
	else if (!strncmp(lex->str_pos, "(", 1))
		default_handle(lex, "(", OPEN_BRACKET);
	else if (!strncmp(lex->str_pos, ")", 1))
		default_handle(lex, ")", CLOSED_BRACKET);
	else
		lex->err = ERR_TOKEN;
	return (NULL);
}

int	default_handle(t_lexer *lex, const char *value, e_token type)
{
	int		len;
	char	*new_word;

	if (!lex || !value)
		return (1);
	len = ft_strlen(value);
	lex->str_pos += len;
	new_word = get_word(value, len);
	push_token(&lex->tokens, new_word, type);
	if (type == HERE_DOC)
	{
		new_word = get_hd_stop_word(lex);
		if (new_word)
			push_token(&lex->tokens, new_word, STRING);
	}
	return (0);
}

