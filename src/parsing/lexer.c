#include "../../inc/parser.h"

char	*poststring_handle()
{
	return (NULL);
}

int	assignment_handle(t_lexer *lex)
{



	return (0);
}

char	*double_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;
	char	*t;

	i = 0;
	new_word = NULL;
	lex->pos++;
	while (lex->pos[i] && lex->pos[i] != '\"')
	{
		if (is_catchar(lex->pos[i]))
		{
			if (new_word)
				new_word = merge_str(new_word, get_word(lex->pos, i));
			else
				new_word = get_word(lex->pos, i);
			lex->pos += i;
			t = scan_token(lex);
			new_word = merge_str(new_word, t);
			i = -1;
		}
		i++;
	}
	if (lex->pos[i] != '\"')
	{
		printf("Double quotes wasn\'t close\n"); // TODO error_handle
		return (NULL);
	}
	if (new_word)
	{
		t = get_word(lex->pos, i);
		new_word = merge_str(new_word, t);
	}
	else
		new_word = get_word(lex->pos, i + 1);
	lex->pos += i + 1;
	return (new_word);
}

char	*single_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*new_word;
	char	*t;

	i = 0;
	new_word = NULL;
	lex->pos++;
	while (lex->pos[i] && lex->pos[i] != '\'')
		i++;
	if (lex->pos[i] != '\'')
	{
		printf("Single quotes wasn\'t close\n"); // TODO error_handle
		return (NULL);
	}
	new_word = get_word(lex->pos, i);
	lex->pos += i + 1;
	if (is_catchar(*lex->pos) || !isspace(*lex->pos))
	{
		t = scan_token(lex);
		new_word = merge_str(new_word, t);
	}
	return (new_word);
}

char	*slash_handle(t_lexer *lex)
{
	char *new_word;
	char *t;

	lex->pos++;
	if (!*lex->pos)
		return (0);
	new_word = get_word(lex->pos, 1);
	lex->pos++;
	if (!isspace(*lex->pos) && *lex->pos)
	{
		t = scan_token(lex);
		new_word = merge_str(new_word, t);
	}
	return (new_word);
}

char	*variable_handle(t_lexer *lex)
{
	int		i;
	char	*t;
	char	*new_word;
	t_item	*var;

	new_word = NULL;
	var = NULL;
	i = 0;
	lex->pos++;
	while (lex->pos[i] &&
		!is_metachar(lex->pos[i]) &&
		!isspace(lex->pos[i]) &&
		!is_catchar(lex->pos[i]))
		i++;
	new_word = get_word(lex->pos, i);
	var = search_item(lex->env, new_word);
	lex->pos += i;
	free(new_word);
	if (!var)
		return (get_word("", 0));
	new_word = get_word(var->value, ft_strlen(var->value));
	if (!is_catchar(*lex->pos) && *lex->pos != '\"')
	{
		t = scan_token(lex);
		new_word = merge_str(new_word, t);
	}
	return (new_word);
}

char	*string_handle(t_lexer *lex)
{
	char	*new_word;
	char	*t;
	int		i;

	i = 0;
	new_word = NULL;
	while (lex->pos[i] && !isspace(lex->pos[i]))
	{
		if (is_metachar(lex->pos[i]))
		{
			new_word = get_word(lex->pos, i);
			lex->pos += i;
			return (new_word);
		}
		if (is_catchar(lex->pos[i]))
		{
			if (new_word)
				new_word = merge_str(new_word, get_word(lex->pos, i));
			else
				new_word = get_word(lex->pos, i);
			lex->pos += i;
			t = scan_token(lex);
			new_word = merge_str(new_word, t);
			i = -1;
		}
		i++;
	}
	if (new_word)
	{
		t = get_word(lex->pos, i);
		new_word = merge_str(new_word, t);
	}
	else
		new_word = get_word(lex->pos, i + 1);
	lex->pos += i;
	return (new_word);
}

int	default_handle(t_lexer *lex, const char *value, e_token type)
{
	int	len;
	char	*new_word;

	if (!lex || !value || !type)
		return (1);
	len = ft_strlen(value);	
	new_word = get_word(value, len);
	push_token(&lex->tokens, new_token(new_word, type));
	lex->pos += len;
	return (0);
}

int	metachar_handle(t_lexer *lex)
{
	if (!strncmp(lex->pos, "||", 2))
		default_handle(lex, "||", OR);
	else if (!strncmp(lex->pos, "|", 1))
		default_handle(lex, "|", PIPE);
	else if (!strncmp(lex->pos, "&&", 2))
		default_handle(lex, "&&", AND);
	else if (!strncmp(lex->pos, ";", 1))
		default_handle(lex, ";", SEMICOLON);
	else if (!strncmp(lex->pos, "<<", 2))
		default_handle(lex, "<<", INPUT_ADD);
	else if (!strncmp(lex->pos, "<", 1))
		default_handle(lex, "<", INPUT_TRUNC);
	else if (!strncmp(lex->pos, ">>", 2))
		default_handle(lex, ">>", OUTPUT_ADD);
	else if (!strncmp(lex->pos, ">", 1))
		default_handle(lex, ">", OUTPUT_TRUNC);
	else
	{
		printf("Unexpected type of token\n");
		return (1);
	}
	return (0);
}

char	*scan_token(t_lexer *lex)
{
	char	*res;

	res = NULL;
	if (!lex || !*lex->pos)
		return (NULL);
	while (isspace(*lex->pos))
		lex->pos++;
	if (!*lex->pos)
		return (NULL);
	if (is_metachar(*lex->pos))
		metachar_handle(lex);
	else if (!strncmp(lex->pos, "\"", 1))
		res = double_quotes_handle(lex);
	else if (!strncmp(lex->pos, "\'", 1))
		res = single_quotes_handle(lex);
	else if (!strncmp(lex->pos, "\\", 1))
		res = slash_handle(lex);
	else if (!strncmp(lex->pos, "$", 1))
		res = variable_handle(lex);
	else if (!is_metachar(*lex->pos))
		res = string_handle(lex);
	else
		return (NULL);
	return (res);
}