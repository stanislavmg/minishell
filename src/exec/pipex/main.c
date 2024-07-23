#include "../exec.h"
#include "pipex.h"

int	pipex()
{
	exec_commands(NULL, NULL, NULL);
	free_args(NULL, 2);
	return (0);
}

int	first_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp)
{
	int	fd;

	if (access(fname, F_OK) || access(fname, R_OK))
		exit_failure(fname, NULL);
	if (!cmd->path && cmd->args[0])
		exit_failure(cmd->args[0], CMD_ERR);
	if (!cmd->path[0])
		exit_failure(cmd->args[0], CMD_ERR);
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit(EXIT_FAILURE);
	ft_close(&pdes[0]);
	if (dup2(fd, STDIN_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	if (dup2(pdes[1], STDOUT_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	ft_close(&pdes[1]);
	if (cmd->path)
		execve(cmd->path, cmd->args, envp);
	exit_failure(cmd->args[0], NULL);
	return (1);
}

int	second_ch(int *pdes, const char *fname, t_cmd *cmd, char **envp)
{
	int	fd;

	if (!access(fname, F_OK) && access(fname, W_OK))
		exit_failure(fname, NULL);
	if (!cmd->path && cmd->args[0])
		exit_failure(cmd->args[0], CMD_ERR);
	if (!cmd->path[0])
		exit(EXIT_FAILURE);
	fd = open(fname, O_TRUNC | O_CREAT | O_RDWR, 0644);
	ft_close(&pdes[1]);
	if (dup2(pdes[0], STDIN_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit_failure(cmd->args[0], NULL);
	ft_close(&pdes[0]);
	if (cmd->path)
		execve(cmd->path, cmd->args, envp);
	exit_failure(cmd->args[0], NULL);
	return (1);
}

void	exec_commands(t_cmd *cmds, char **argv, char **envp)
{
	pid_t	ps1;
	pid_t	ps2;
	int		status;
	int		pdes[2];

	pipe(pdes);
	ps1 = fork();
	if (!ps1)
		first_ch(pdes, argv[1], cmds, envp);
	ps2 = fork();
	if (!ps2)
		second_ch(pdes, argv[4], ++cmds, envp);
	if (ps1 == -1 || ps2 == -1)
		return ;
	ft_close(&pdes[0]);
	ft_close(&pdes[1]);
	waitpid(ps1, &status, 0);
	waitpid(ps2, &status, 0);
}
