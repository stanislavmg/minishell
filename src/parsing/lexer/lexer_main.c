#include "parser.h"

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_token 	*list;
	t_list		*env_list;
	int 		i;

	lex = NULL;
	env_list = new_env_list(env);
	lex = new_lexer(av[1], env_list);
	init_list(lex);
	if (!lex || !lex->tokens)
	{
		printf("No tokens\n");
		return (1);
	}
	i = 0;
	print_tokens(lex->tokens);
	return (0);
}