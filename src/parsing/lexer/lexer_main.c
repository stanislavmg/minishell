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
	t_token 	*list;
	t_env		*env_list;
	int 		i;

	lex = NULL;
	env_list = create_envlist(env);
	lex = new_lexer(av[1], env_list);
	init_list(lex);
	if (!lex || !lex->tokens)
	{
		printf("No tokens\n");
		return (1);
	}
	i = 0;
	while (lex->tokens)
	{
		list = lex->tokens->data;
		lex->tokens = lex->tokens->next;
		printf("#%d cur_token_pos: %s\ntype: %s\n\n", i, list->word, get_type(list->type));
		i++;
	}
	return (0);
}