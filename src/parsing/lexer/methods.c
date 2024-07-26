#include "parser.h"

t_lexer	*new_lexer(char *str, t_env *env_list)
{
	t_lexer *lex;

	if (!str)
		return (NULL);
	lex = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!lex)
		return (NULL);
	lex->str_pos = str;
	lex->input_str = str;
	lex->env = env_list;
	return (lex);
}

t_word_list	*new_token(char *word, e_token type)
{
	t_word_list *new_word;

	new_word = (t_word_list *)ft_calloc(1, sizeof(t_word_list));
	if (!new_word)
		return (NULL);
	new_word->type = type;
	new_word->word = word;
	return (new_word);
}

int	push_token(t_word_list **list, t_word_list *new)
{
	t_word_list	*t;
	
	if (!list)
		return (1);
	t = *list;
	if (!*list)
		*list = new;
	else
	{
		while (t->next)
			t = t->next;
		t->next = new;
		new->prev = t;
	}
	return (0);
}

void	error_handle(t_lexer *lex)
{
	if (ERR_MEM == lex->err)
		printf("minishell: No memory\n");
	else if (ERR_QUOTE == lex->err)
		printf("minishell: Quotes wasn\'t close\n");
	else if (ERR_TOKEN == lex->err)
		printf("minishell: syntax error near unexpected token `%c'\n", *lex->str_pos);
	else
		printf("minishell: Undefined error. Abort!\n");
}

void	free_lexer(t_lexer *lex)
{
	t_word_list *t;

	t = lex->tokens;
	while (t)
	{
		lex->tokens = t->next;
		free(t->word);
		free(t);
		t = lex->tokens;
	}
	free(lex);
}

int	init_word_list(t_lexer *lex)
{
	char	*new_word;

	new_word = NULL;
	while(*lex->str_pos)
	{
		new_word = scan_token(lex);
		if (lex->err)
		{
			error_handle(lex);
			free_lexer(lex);
			return (1);
		}
		else if (new_word && *new_word) //FIXME leak when return ""
			push_token(&lex->tokens, new_token(new_word, STRING));
	}
	return (0);
}