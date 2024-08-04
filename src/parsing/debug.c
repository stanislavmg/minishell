#include "parser.h"

void	print_tokens(t_list *tokens)
{
	int i = 1;

	while (tokens)
	{
		printf("#%d token_word: %s\ntype: %s\n\n", i, 
				((t_token*)tokens->data)->word,
				get_type(((t_token*)tokens->data)->type));
		i++;
		tokens = tokens->next;
	}
}

const char *get_type(int type)
{
	if (END == type)
		return ("END");
	else if (IO_FILE == type)
		return ("IO_FILE");
	else if (REDIRECT == type)
		return ("REDIRECT");
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
	else if (ASSIGNMENT == type)
		return ("ASSIGNMENT");
	return (NULL);
}