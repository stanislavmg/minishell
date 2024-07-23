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