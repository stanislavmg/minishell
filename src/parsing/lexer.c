#include "parser.h"

char	*str_poststring_handle(t_lexer *lex, char *lhs, int i)
{
	char *res;
	char *t;

	res = NULL;
	if (lhs)
		res = merge_str(lhs, get_word(lex->str_pos, i));
	else
		res = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (lex->in_qoutes && (*lex->str_pos == '\"' || *lex->str_pos == '\''))
	{
		lex->in_qoutes = 0;
		lex->str_pos++;
	}
	t = scan_token(lex);
	res = merge_str(res, t);
	return (res);
}

char	*double_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;

	i = 0;
	new_word = NULL;
	lex->str_pos++;
	lex->in_qoutes = 1;
	while (lex->str_pos[i] && lex->str_pos[i] != '\"')
	{
		if ('$' == lex->str_pos[i] || '\\' == lex->str_pos[i])
		{
			new_word = str_poststring_handle(lex, new_word, i);
			i = -1;
		}
		i++;
	}
	if (lex->str_pos[i] != '\"')
	{
		lex->err = ERR_QUOTE;
		return (NULL);
	}
	if (is_catchar(lex->str_pos[i + 1]))
	{
		new_word = str_poststring_handle(lex, new_word, i);
		return (new_word);
	}
	else
	{
		if (new_word)
			new_word = merge_str(new_word, get_word(lex->str_pos, i));
		else
			new_word = get_word(lex->str_pos, i);
	}
	lex->in_qoutes = 0;
	lex->str_pos += i + 1;
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
	if (is_catchar(*lex->str_pos) || !isspace(*lex->str_pos))
		new_word = str_poststring_handle(lex, new_word, 0);
	return (new_word);
}

char	*slash_handle(t_lexer *lex) // ` " $
{
	char *new_word;
	char *t;

	lex->str_pos++;
	if (lex->in_qoutes)
	{
		if (*lex->str_pos == '`' ||
			*lex->str_pos == '\"' ||
			*lex->str_pos == '$' ||
			*lex->str_pos == '\\')
		{
			new_word = get_word(lex->str_pos, 1);
			lex->str_pos++;
			return (new_word);
		}
		else
			return (ft_strdup("\\"));
	}
	if (!*lex->str_pos)
		return (get_word("", 0));
	new_word = get_word(lex->str_pos, 1);
	lex->str_pos++;
	if (*lex->str_pos && !isspace(*lex->str_pos) && !is_metachar(*lex->str_pos))
	{
		t = scan_token(lex);
		new_word = merge_str(new_word, t);
	}
	return (new_word);
}

char	*variable_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;
	t_env	*var;

	new_word = NULL;
	var = NULL;
	i = 0;
	lex->str_pos++;
	while (lex->str_pos[i] &&
		!is_metachar(lex->str_pos[i]) &&
		!isspace(lex->str_pos[i]) &&
		!is_catchar(lex->str_pos[i]))
		i++;
	new_word = get_word(lex->str_pos, i);
	var = list_search(lex->env, new_word);
	lex->str_pos += i;
	free(new_word);
	if (!var)
		return (get_word("", 0));
	new_word = get_word(var->value, ft_strlen(var->value));
	if (lex->in_qoutes)
		return (new_word);
	if (is_catchar(*lex->str_pos))
		new_word = str_poststring_handle(lex, new_word, 0);
	return (new_word);
}

char	*string_handle(t_lexer *lex)
{
	char	*new_word;
	int		i;

	i = 0;
	new_word = NULL;
	while (lex->str_pos[i] && !is_metachar(lex->str_pos[i]) && !isspace(lex->str_pos[i]))
	{
		if (is_catchar(lex->str_pos[i]))
		{
			new_word = str_poststring_handle(lex, new_word, i);
			i = -1;
		}
		i++;
	}
	if (new_word)
		new_word = merge_str(new_word, get_word(lex->str_pos, i));
	else
		new_word = get_word(lex->str_pos, i + 1);
	lex->str_pos += i;
	return (new_word);
}

int	default_handle(t_lexer *lex, const char *value, e_token type)
{
	int		len;
	char	*new_word;

	if (!lex || !value || !type)
		return (1);
	len = ft_strlen(value);	
	new_word = get_word(value, len);
	push_token(&lex->tokens, new_token(new_word, type));
	lex->str_pos += len;
	return (0);
}

char	*metachar_handle(t_lexer *lex)
{
	if (!strncmp(lex->str_pos, "||", 2))
		default_handle(lex, "||", OR);
	else if (!strncmp(lex->str_pos, "|", 1))
		default_handle(lex, "|", PIPE);
	else if (!strncmp(lex->str_pos, "&&", 2))
		default_handle(lex, "&&", AND);
	else if (!strncmp(lex->str_pos, ";", 1))
		default_handle(lex, ";", SEMICOLON);
	else if (!strncmp(lex->str_pos, "<<", 2))
		default_handle(lex, "<<", HERE_DOC);
	else if (!strncmp(lex->str_pos, "<", 1))
		default_handle(lex, "<", INPUT_TRUNC);
	else if (!strncmp(lex->str_pos, ">>", 2))
		default_handle(lex, ">>", OUTPUT_ADD);
	else if (!strncmp(lex->str_pos, ">", 1))
		default_handle(lex, ">", OUTPUT_TRUNC);
	else
		lex->err = ERR_TOKEN;
	return (NULL);
}

char	*scan_token(t_lexer *lex)
{
	char	*res;

	res = NULL;
	if (lex->err)
		return (NULL);
	if (!lex || !*lex->str_pos)
		return (NULL);
	while (isspace(*lex->str_pos))
		lex->str_pos++;
	if (!*lex->str_pos)
		return (NULL);
	if (is_metachar(*lex->str_pos))
		metachar_handle(lex);
	else if (!strncmp(lex->str_pos, "\"", 1))
		res = double_quotes_handle(lex);
	else if (!strncmp(lex->str_pos, "\'", 1))
		res = single_quotes_handle(lex);
	else if (!strncmp(lex->str_pos, "\\", 1))
		res = slash_handle(lex);
	else if (!strncmp(lex->str_pos, "$", 1))
		res = variable_handle(lex);
	else if (!is_metachar(*lex->str_pos))
		res = string_handle(lex);
	else
		return (NULL);
	return (res);
}