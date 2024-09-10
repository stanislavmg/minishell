/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:20:26 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 10:55:16 by sgoremyk         ###   ########.fr       */
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
			push_token(&lex->tokens, new_word, ARG);
	}
	if (!string_is_spaces(lex->input))
		add_history(lex->input);
	if (lex->err == ERR_QUOTE)
	{
		print_msh_err("unclosed quote");
		ft_lstclear(&lex->tokens, fret_token);
	}
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
	if (!ft_strncmp(lex->str_pos, "\"", 1))
		new_word = double_quotes_handle(lex);
	else if (is_metachar(*lex->str_pos))
		metachar_handle(lex);
	else if (!ft_strncmp(lex->str_pos, "\'", 1))
		new_word = single_quotes_handle(lex);
	else if (!ft_strncmp(lex->str_pos, "\\", 1))
		new_word = slash_handle(lex);
	else if (!ft_strncmp(lex->str_pos, "$", 1))
		new_word = variable_handle(lex);
	else
		new_word = string_handle(lex);
	return (new_word);
}

char	*redirect_handle(t_lexer *lex)
{
	if (!ft_strncmp(lex->str_pos, "<<", 2))
		default_handle(lex, "<<", HERE_DOC);
	else if (!ft_strncmp(lex->str_pos, "<", 1))
		default_handle(lex, "<", INPUT_TRUNC);
	else if (!ft_strncmp(lex->str_pos, ">>", 2))
		default_handle(lex, ">>", OUTPUT_ADD);
	else if (!ft_strncmp(lex->str_pos, ">", 1))
		default_handle(lex, ">", OUTPUT_TRUNC);
	return (NULL);
}

char	*metachar_handle(t_lexer *lex)
{
	if (*lex->str_pos == '<' || *lex->str_pos == '>')
		redirect_handle(lex);
	else if (!ft_strncmp(lex->str_pos, "||", 2))
		default_handle(lex, "||", OR);
	else if (!ft_strncmp(lex->str_pos, "|", 1))
		default_handle(lex, "|", PIPE);
	else if (!ft_strncmp(lex->str_pos, "&&", 2))
		default_handle(lex, "&&", AND);
	else if (!ft_strncmp(lex->str_pos, ";", 1))
		default_handle(lex, ";", SEMICOLON);
	else if (!ft_strncmp(lex->str_pos, "(", 1))
		default_handle(lex, "(", OPEN_BRACKET);
	else if (!ft_strncmp(lex->str_pos, ")", 1))
		default_handle(lex, ")", CLOSED_BRACKET);
	else
		lex->err = ERR_TOKEN;
	return (NULL);
}

int	default_handle(t_lexer *lex, const char *value, t_token_type type)
{
	int		len;
	char	*new_word;

	if (!lex || !value)
		return (1);
	len = ft_strlen(value);
	lex->str_pos += len;
	new_word = get_word(value, len);
	if (type == HERE_DOC)
	{
		while (ft_isspace(*lex->str_pos))
			lex->str_pos++;
		if (*lex->str_pos == '\'' || *lex->str_pos == '\"')
			type = EXPAND_HERE_DOC;
		push_token(&lex->tokens, new_word, type);
		new_word = get_hd_stop_word(lex, NULL);
		if (new_word)
			push_token(&lex->tokens, new_word, ARG);
	}
	else
		push_token(&lex->tokens, new_word, type);
	return (0);
}
