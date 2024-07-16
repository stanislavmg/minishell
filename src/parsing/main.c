#include "../../inc/parser.h"

const char *get_type(e_token type)
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
	else if (INPUT_ADD == type)
		return ("INPUT_ADD");
	else if (INPUT_TRUNC == type)
		return ("INPUT_TRUNC");
	else if (OUTPUT_ADD == type)
		return ("OUTPUT_ADD");
	else if (OUTPUT_TRUNC == type)
		return ("OUTPUT_TRUNC");
	return (NULL);
}

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_word_list *list;
	char *s = ft_strdup("minishell$> ");
	t_table 	*table;
	char 		**t;
	int 		i;

	table = create_table(CAPACITY);
	if (!table)
		return (1);
	i = 1;
	while (env[i])
	{
		t = ft_split(env[i], '=');
		add_item(table, t[0], t[1]);
		//printf("Key %s\nValue %s\nhas been added\n\n", t[0], t[1]);
		if (t[0])
			free(t[0]);
		if (t[1])
			free(t[1]);
		free(t);
		i++;
	}
//	printf ("%s\n", rl);
	//printf ("\nstr in programm = %s\n\n", av[1]);
	char *res;
	while (1)
	{
		res = readline(s);
		if (!strcmp(res, "exit"))
			break ;
		lex = new_lexer(res);
		lex->env = table;
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
		free_lexer(lex);
	}
	return (0);
}