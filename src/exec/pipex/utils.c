#include "pipex.h"

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

int	ft_close(int *fd)
{
	if (*fd == -1)
		return (0);
	if (close(*fd) == -1)
		return (-1);
	*fd = -1;
	return (0);
}

void	exit_failure(const char *cmd, const char *message)
{
	if (message)
	{
		write(2, "pipex: ", 7);
		ft_putstr_fd((char *)cmd, 2);
		write(2, ": ", 2);
		ft_putstr_fd((char *)message, 2);
	}
	else
		perror(cmd);
	exit(EXIT_FAILURE);
}
