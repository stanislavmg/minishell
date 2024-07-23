#include "parser.h"
#include "minishell.h"

const char *get_type(int type)
{
	if (UNDEFINED == type)
		return ("UNDEFINED");
	else if (STRING == type)
		return ("STRING");
	else if (COMMAND == type)
		return ("COMMAND");
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
	else if (HERE_DOC == type)
		return ("HERE_DOC");
	else if (INPUT_TRUNC == type)
		return ("INPUT_TRUNC");
	else if (OUTPUT_ADD == type)
		return ("OUTPUT_ADD");
	else if (OUTPUT_TRUNC == type)
		return ("OUTPUT_TRUNC");
	else if (OPEN_BRACKET == type)
		return ("OPEN_BRACKET");
	else if (CLOSED_BRACKET == type)
		return ("CLOSED_BRACKET");
	return (NULL);
}

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_word_list *list;
	char *s = ft_strdup("minishell$> ");
	char 		**t;
	int 		i;

	i = 1;
	
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
		lex->token_pos = lex->tokens;
		build_AST(lex);
		free_lexer(lex);
	}
	return (0);
}