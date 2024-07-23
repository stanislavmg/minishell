# include "exec.h"

int	start_exec_cmds(t_cmd *root)
{
	int	exit_code;

	exit_code = 0;
	if (COMMAND == root->type)
		exit_code = exec_cmd((t_exec_cmd *)root); // TODO
	else if (PIPE == root->type)
		exit_code = pipe_exec((t_ast *)root); // TODO
	else if (OR == root->type)
	{
		exit_code = start_exec_cmds(((t_ast *)root)->left);
		if (exit_code)
			exit_code = start_exec_cmds(((t_ast *)root)->right);
	}
	else if (AND == root->type)
	{
		exit_code = start_exec_cmds(((t_ast *)root)->left);
		if (!exit_code)
			exit_code = start_exec_cmds(((t_ast *)root)->right);
	}
	else if (SEMICOLON == root->type)
	{
		exit_code = start_exec_cmds(((t_ast *)root)->left);
		exit_code = start_exec_cmds(((t_ast *)root)->right);
	}
	else
		printf("Unexpected type of token\n");
	return (exit_code);
}

int	exec_cmd(t_exec_cmd *cmd)
{
	int	fdes[2];

	if (cmd->file_in)
		fdes[0] = ft_open(cmd->file_in); // TODO
	if (cmd->file_out)
		fdes[1] = ft_open(cmd->file_out);
	execve(cmd->path, cmd->argv, cmd->env);
		
	return (0);
}

