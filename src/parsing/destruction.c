#include "parser.h"

int	free_cmd(t_exec_cmd *cmd)
{
	free_arr(cmd->argv);
	free(cmd->in_fname);
	free(cmd->out_fname);
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

void	free_tree(t_ast *root)
{
	if (!root)
		return ;
	if (root->type != COMMAND && root->type != VARIABLE)
		free_tree(root->left);
	else if (root->type == COMMAND)
		free_cmd((t_exec_cmd *)root);
	else if (root->type == VARIABLE)
	{
		free(((t_var *)root)->key);
		free(((t_var *)root)->value);
	}
	if (root->type != COMMAND && root->type != VARIABLE)
		free_tree(root->right);
	free(root);
}
