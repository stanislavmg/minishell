#include "parser.h"
int	is_metachar(char ch)
{
	return ('|' == ch || '&' == ch || ';' == ch ||
			'<' == ch || '>' == ch || ' ' == ch ||
			'\t' == ch || '\n' == ch || '$' == ch); // += ( and )
}

// lexer methods
t_lexer	*new_lexer(char *str)
{
	t_lexer *lex;

	if (!str)
		return (NULL);
	lex = (t_lexer *)malloc(sizeof(t_lexer));
	if (!lex)
		return (NULL);
	lex->tokens = NULL;
	lex->pos = str;
	lex->str = str;
	return (lex);
}

t_word_list	*new_token(const char *word, e_token type)
{
	t_word_list *new_word;

	new_word = (t_word_list *)ft_calloc(1, sizeof(t_word_list));
	if (!new_word)
		return (NULL);
	new_word->type = type;
	new_word->word = ft_strdup(word);
	return (new_word);
}

int	push_token(t_word_list **list, t_word_list *new)
{
	t_word_list	*t;

	if (!new || !list)
		return (1);
	t = *list;
	if (!*list)
		*list = new;
	else
	{
		while (t->next)
			t = t->next;
		t->next = new;
	}
	return (0);
}

int	double_quotes_handle(t_lexer *lex)
{
	return (0);
}

int	single_quotes_handle(t_lexer *lex)
{
	int		i;
	char	*tmp_str;

	i = 0;
	lex->pos++;
	while (lex->pos[i] && lex->pos[i] != '\'')
		i++;
	if (lex->pos[i] != '\'')
		return (1);
	tmp_str = (char *)malloc(i + 1);
	ft_strlcpy(tmp_str, lex->pos, i + 1);
	push_token(&lex->tokens, new_token(tmp_str, STR_ONE_Q));
	lex->pos += i + 1;
	free(tmp_str);
	return (0);
}

int	slash_handle(t_lexer *lex)
{
	return (0);
}

int	variable_handle(t_lexer *lex)
{
	int	i;
	char	*tmp_str;

	i = 0;
	lex->pos++;
	while (lex->pos[i] && !is_metachar(lex->pos[i]) && !isspace(lex->pos[i]))
		i++;
	tmp_str = (char *)malloc(i + 2);
	ft_strlcpy(tmp_str, lex->pos, i + 1);
	push_token(&lex->tokens, new_token(tmp_str, VARIABLE));
	lex->pos += i;
	free(tmp_str);
	return (0);
}

int	string_handle(t_lexer *lex)
{
	int		i;
	char	*tmp_str;

	i = 0;
	while (lex->pos[i] && !isspace(lex->pos[i]))
	{
		if (is_metachar(lex->pos[i]))
		{
			tmp_str = (char *)malloc(i + 2);
			ft_strlcpy(tmp_str, lex->pos, i + 1);
			push_token(&lex->tokens, new_token(tmp_str, STRING));
			lex->pos += i;
			free(tmp_str);
			init_tokens(lex);
			i = 0;
		}
		i++;
	}
	tmp_str = (char *)malloc(i + 2);
	ft_strlcpy(tmp_str, lex->pos, i + 1);
	push_token(&lex->tokens, new_token(tmp_str, STRING));
	lex->pos += i;
	free(tmp_str);
	return (0);
}

int	default_handle(t_lexer *lex, const char *value, e_token type)
{
	if (!lex || !value || !type)
		return (1);
	push_token(&lex->tokens, new_token(value, type));
	lex->pos += ft_strlen(value);
	return (0);
}

int	init_tokens(t_lexer *lex)
{
	while (*lex->pos)
	{
		if (isspace(*lex->pos))
			lex->pos++;
		else if (!strncmp(lex->pos, "||", 2))
			default_handle(lex, "||", OR);
		else if (!strncmp(lex->pos, "|", 1))
			default_handle(lex, "|", PIPE);
		else if (!strncmp(lex->pos, "&&", 2))
			default_handle(lex, "&&", AND);
		else if (!strncmp(lex->pos, ";", 1))
			default_handle(lex, ";", SEMICOLON);
		else if (!strncmp(lex->pos, "~", 1))
			default_handle(lex, "~", TILDA);
		else if (!strncmp(lex->pos, "\"", 1))
			double_quotes_handle(lex);
		else if (!strncmp(lex->pos, "\'", 1))
			single_quotes_handle(lex);
		else if (!strncmp(lex->pos, "\\", 1))
			slash_handle(lex);
		else if (!strncmp(lex->pos, "$", 1))
			variable_handle(lex);
		else if (!is_metachar(*lex->pos))
			string_handle(lex);
		else
			return (1);
	}
	return (0);
}