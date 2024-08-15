#include "parser.h"

char	*scan_token(t_lexer *lex)
{
	char	*new_word;

	new_word = NULL;
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
		new_word = double_quotes_handle(lex);
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
	if (is_redirect(type))
		new_word = scan_token(lex);
	else
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
	while (lex->str_pos[i] &&
		!is_metachar(lex->str_pos[i]) &&
		!isspace(lex->str_pos[i]))
	{
		if (is_catchar(lex->str_pos[i]))
		{
			new_word = poststring_handle(lex, new_word, i);
			i = -1;
		}
		i++;
	}
	if (new_word)
		new_word = merge_str(new_word, get_word(lex->str_pos, i));
	else
		new_word = get_word(lex->str_pos, i);
	lex->str_pos += i;
	if (ft_strchr(new_word, '*'))
	{
		push_token(&lex->tokens, new_word, WILDCARD);
		return (NULL);
	}
	return (new_word);
}

char	*poststring_handle(t_lexer *lex, char *lhs, int i)
{
	char *res;
	char *t;

	res = NULL;
	t = NULL;
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

	new_word = NULL;
	lex->str_pos++;
	lex->in_qoutes = 1;
	while (lex->in_qoutes && !lex->err)
	{
		i = 0;
		while (lex->str_pos[i] && lex->str_pos[i] != '\"')
		{
			if ('$' == lex->str_pos[i] || '\\' == lex->str_pos[i])
			{
				new_word = merge_str(new_word, get_word(lex->str_pos, i));
				lex->str_pos += i;
				new_word = merge_str(new_word, scan_token(lex));
				i = -1;
			}
			i++;
		}
		if (lex->str_pos[i] == '\"')
		{
			new_word = merge_str(new_word, get_word(lex->str_pos, i));
			lex->str_pos += i + 1;
			if (*lex->str_pos == '\"')
				lex->str_pos++;
			else
				lex->in_qoutes = 0;
		}
		else
			lex->err = ERR_QUOTE;
	}
	if (is_catchar(*lex->str_pos))
		new_word = merge_str(new_word, scan_token(lex));
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
		new_word = poststring_handle(lex, new_word, 0);
	return (new_word);
}

int	is_screning_ch(int ch)
{
	return (ch == '`' || ch == '\"' || ch == '$' || ch == '\\');
}

char	*slash_handle(t_lexer *lex)
{
	char *new_word;
	char *t;

	lex->str_pos++;
	if (lex->in_qoutes)
	{
		if (is_screning_ch(*lex->str_pos))
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

	i = 0;
	var = NULL;
	new_word = NULL;
	lex->str_pos++;
	while (lex->str_pos[i] &&
		!is_metachar(lex->str_pos[i]) &&
		!isspace(lex->str_pos[i]) &&
		!is_catchar(lex->str_pos[i]))
		i++;
	new_word = get_word(lex->str_pos, i);
	var = get_env(lex->env, new_word);
	lex->str_pos += i;
	free(new_word);
	if (!var)
		return (get_word("", 0));
	new_word = get_word(var->value, ft_strlen(var->value));
	if (lex->in_qoutes)
		return (new_word);
	if (is_catchar(*lex->str_pos))
		new_word = poststring_handle(lex, new_word, 0);
	return (new_word);
}

int var_assignment(t_lexer *lex)
{
	char	*var_name;
	char	*var_value;

	var_name = get_var_name(lex->str_pos);
	if (!var_name)
		return (1);
	lex->str_pos += ft_strlen(var_name);
	if ('(' == *lex->str_pos)
	{
		lex->str_pos++;
		while (isspace(*lex->str_pos))
			lex->str_pos++;
		var_value = scan_token(lex);
		if(')' == *lex->str_pos)
			lex->str_pos++;
	}
	else if (isspace(*lex->str_pos))
		var_value = ft_strdup("");
	else
		var_value = scan_token(lex);
	lex->str_pos += ft_strlen(var_value);
	return (0);
}

int	insert_variable(t_list *list_env, t_env *new_var)
{
	t_env	*search_var;

	if (!new_var)
		return (1);
	search_var = get_env(list_env, new_var->key);
	{
		free(search_var->value);
		search_var->value = new_var->value;
		new_var->value = NULL;
		free(new_var->key);
		free(new_var);
	}
	return (0);
}