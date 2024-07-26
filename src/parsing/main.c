#include "parser.h"
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_env		*env_lst;
	t_word_list *list;
	t_parser	*pars_info;
	char 		*res;
	int 		i = 1;
	char 		*s = "minishell$> ";

	env_lst = create_envlist(env);
	while (1)
	{
		res = readline(s);
		if (!strcmp(res, "exit"))
			break ;
		lex = new_lexer(res, env);
		if (init_word_list(lex))
			continue;
		list = lex->tokens;
		i = 0;
		while (list)
		{
			printf("#%d token: %s\ntype: %s\n\n", i, list->word, get_type(list->type));
			i++;
			list = list->next;
		}
		pars_info = init_parser(lex);
		build_AST(lex);
		free_lexer(lex);
	}
	return (0);
}