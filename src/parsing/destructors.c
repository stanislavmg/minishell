#include "parser.h"

int	free_cmd(t_exec_cmd *cmd)
{
	free_arr(cmd->argv);
	free(cmd->path);
	return (0);
}

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	if (!arr)
		return ;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

void	free_parser(t_parser *parser)
{
	free_arr(parser->paths);
	free(parser);	
}

void	free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->type != COMMAND && root->type != VARIABLE	&& root->type != IO_FILE)
		free_ast((t_ast *)root->left);
	else if (root->type == COMMAND)
		free_cmd((t_exec_cmd *)root);
	else if (root->type == VARIABLE)
	{
		free(((t_var *)root)->key);
		free(((t_var *)root)->value);
	}
	else if (root->type == IO_FILE)
		free(((t_redir *)root)->fname);		
	if (root->type != COMMAND && root->type != VARIABLE	&& root->type != IO_FILE)
		free_ast((t_ast *)root->right);
	free(root);
}
