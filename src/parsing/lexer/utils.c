#include "parser.h"

int	is_cmd_delimeter(e_token type)
{ 
	return (AND == type || PIPE == type ||
			OR == type || SEMICOLON == type ||
			CLOSED_BRACKET == type || OPEN_BRACKET == type);
}

int	is_redirect(e_token type)
{
	return (HERE_DOC == type || INPUT_TRUNC == type ||
			OUTPUT_ADD == type || OUTPUT_TRUNC == type);
}

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
	else if (ASSIGNMENT == type)
		return ("ASSIGNMENT");
	return (NULL);
}

pid_t	ft_fork(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: ");
		return (-1);
	}
	return (pid);
}