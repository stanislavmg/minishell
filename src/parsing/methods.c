#include "../../inc/parser.h"

t_lexer	*new_lexer(char *str)
{
	t_lexer *lex;

	if (!str)
		return (NULL);
	lex = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!lex)
		return (NULL);
	lex->pos = str;
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
	}
	return (0);
}

int	init_word_list(t_lexer *lex)
{
	char	*tmp_str;

	tmp_str = NULL;
	while(*lex->pos)
	{
		tmp_str = scan_token(lex);
		if (tmp_str)
			push_token(&lex->tokens, new_token(tmp_str, STRING));
	}
	return (0);
}