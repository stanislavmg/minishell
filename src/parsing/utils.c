#include "../../inc/parser.h"


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