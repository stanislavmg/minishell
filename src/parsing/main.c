#include "parser.h"

const char *get_type (e_token type)
{
	if (STRING == type)
		return ("STRING");
	else if(OR == type)
		return ("OR");
	else if (SEMICOLON == type)
		return ("SEMICOLON");
	else if (PIPE == type)
		return ("PIPE");
	else if (AND == type)
		return ("AND");
	else if (VARIABLE == type)
		return ("VARIABLE");
	else if (STR_ONE_Q == type)
		return ("STR_ONE_Q");
	else if (CHARACTER == type)
		return ("STR_ONE_Q");
	return (NULL);
}

int	main(int ac, char **av)
{
	t_lexer *lex;
	t_word_list *list;
	int i = 1;

	lex = new_lexer(av[1]);
	init_tokens(lex);
	list = lex->tokens;
	while (list)
	{
		printf("#%d token: %s\ntype: %s\n\n", i, list->word, get_type(list->type));
		i++;
		list = list->next;
	}
	return (0);
}