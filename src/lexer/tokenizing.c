/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 18:11:53 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/22 13:17:49 by sgoremyk         ###   ########.fr       */
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
		if (new_word && !*new_word)
			free(new_word);
		else if (new_word)
			push_token(&lex->tokens, new_word, STRING);
		if (!*lex->str_pos && ft_lstlast(lex->tokens)
			&& ((t_token *)ft_lstlast(lex->tokens)->data)->type == PIPE)
			add_new_input(lex);
	}
	if (lex->err)
	{
		print_msh_err(ft_lstlast(lex->tokens)->data);
		ft_lstclear(&lex->tokens, free_token);
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

char	*get_hd_stop_word(t_lexer *lex)
{
	int		i;
	char	*stop_word;
	char	qoutes;

	i = 0;
	qoutes = 0;
	stop_word = NULL;
	while (ft_isspace(*lex->str_pos))
		lex->str_pos++;
	if (*lex->str_pos == '\'' || *lex->str_pos == '\"')
	{
		qoutes = *lex->str_pos;
		lex->str_pos++;
	}
	while (lex->str_pos[i] && lex->str_pos[i] != qoutes
			&& !ft_isspace(lex->str_pos[i])
			&& !is_metachar(lex->str_pos[i])
			&& !is_redirectchar(lex->str_pos[i]))
		i++;
	if (qoutes && lex->str_pos[i] != qoutes)
		lex->err = ERR_SYNTAX;
	stop_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (qoutes)
		lex->str_pos++;
	if (*lex->str_pos && !ft_isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
		stop_word = merge_str(stop_word, get_hd_stop_word(lex));
	return (stop_word);
}

int	is_token_delimeter(int ch)
{
	return (ch == 0 || ft_isspace(ch) || is_metachar(ch) || is_redirect(ch));
}

int	default_handle(t_lexer *lex, const char *value, e_token type)
{
	int		len;
	char	*new_word;
	t_list	*last_token;
	
	if (!lex || !value)
		return (1);
	last_token = NULL;
	len = ft_strlen(value);
	lex->str_pos += len;
	if (is_redirect(type))
	{
		push_token(&lex->tokens, NULL, type);
		last_token = ft_lstlast(lex->tokens);
		if (type == HERE_DOC)
			((t_token *)last_token->data)->word = get_hd_stop_word(lex);
		else
			((t_token *)last_token->data)->word = scan_token(lex);
		return (0);
	}
	new_word = get_word(value, len);
	if (!new_word)
	{
		lex->err = ERR_SYNTAX;
		new_word = get_word(value, len);
	}
	push_token(&lex->tokens, new_word, type);
	return (0);
}

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
	if (ft_strchr(new_word, '*') && ft_isspace(*lex->str_pos)) // FIXME if *lex->str_pos == 0
	{
		expand_wildcard(lex, new_word);
		free(new_word);
		new_word = NULL;
	}
	if (is_catchar(*lex->str_pos))
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
	if (is_catchar(*lex->str_pos) || !ft_isspace(*lex->str_pos))
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
	if (is_catchar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}

int	is_screening_ch(int ch)
{
	return (ch == '`' || ch == '\"' || ch == '$' || ch == '\\');
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
	else
	{
		new_word = get_word(lex->str_pos, 1);
		if (*lex->str_pos)
			lex->str_pos++;
	}
	if(is_catchar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
	return (new_word);
}

char	*variable_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;
	t_env	*var;

	i = 0;
	var = NULL;
	new_word = NULL;
	lex->str_pos++;
	if (*lex->str_pos == '?')
	{
		new_word = ft_strdup("?");
		i = 1;
	}
	else
	{
		while (!is_token_delimeter(lex->str_pos[i]) && !is_catchar(lex->str_pos[i]))
			i++;
		if (i == 0)
			new_word = get_word("$", 1);
		else
			new_word = get_word(lex->str_pos, i);
	}
	lex->str_pos += i;
	if (i){
	var = get_env(lex->env, new_word);
	free(new_word);
	if (!var)
		new_word = get_word("", 0);
	else
		new_word = ft_strdup(var->value);
	}
	if (lex->in_qoutes)
		new_word = merge_str(new_word, double_quotes_handle(lex));
	else if (is_catchar(*lex->str_pos) || !ft_isspace(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));

	return (new_word);
}
