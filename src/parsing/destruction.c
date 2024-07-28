#include "parser.h"

int	free_cmd(t_exec_cmd *cmd)
{
	free_arr(cmd->argv);
	free(cmd->argv);
	free(cmd->in_fname);
	free(cmd->out_fname);
	free(cmd);
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

}

void	free_tree(t_ast root)
{
	
}