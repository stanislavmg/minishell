#include "parser.h"

void print_env(t_env *t)
{
	while (t)
	{
		printf("Key: %s\nValue: %s\n\n", t->key, t->value);
		t = t->next;
	}
}

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_word_list *list;
	t_env		*env_list;
	int 		i;


	env_list = create_envlist(env);
	lex = new_lexer(av[1], env_list);
	init_word_list(lex);
	if (!lex || !lex->token_pos)
	{
		printf("No tokens\n");
		return (1);
	}
	list = lex->tokens;
	i = 0;
	while (list)
	{
		printf("#%d token: %s\ntype: %s\n\n", i, list->word, get_type(list->type));
		i++;
		list = list->next;
	}
	return (0);
}