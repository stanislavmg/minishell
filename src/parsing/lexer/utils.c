#include "parser.h"

int	is_cmd_delimeter(e_token type)
{ 
	return (AND == type || PIPE == type ||
			OR == type || SEMICOLON == type);
}

int	is_redirect(e_token type)
{
	return (HERE_DOC == type || INPUT_TRUNC == type ||
			OUTPUT_ADD == type || OUTPUT_TRUNC == type);
}

void print_tree(t_ast *root) {
    if (root != NULL) {
        if (root->type == COMMAND) {
            printf("%s\n", ((t_exec_cmd *)root)->argv[0]);
            printf("type of node: %s\n", get_type(root->type));
            return;
        }
        printf("type of node: %s\n", get_type(root->type));
        if (root->left != NULL) {
            printf("left node = ");
            print_tree((t_ast *)root->left);
        }
        if (root->right != NULL) {
            printf("right node = ");
            print_tree((t_ast *)root->right);
        }
    }
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