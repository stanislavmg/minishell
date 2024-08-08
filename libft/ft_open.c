# include "libft.h"

int	ft_open(char *fname, int mode)
{
	int	fd;

	fd = 0;
	/* If mode is readonly then this is input file */
	if (mode == O_RDONLY)
	{
		if (access(fname, F_OK) || access(fname, R_OK))
			fd = -1;
	}
	/* Else output file */
	else
	{
		if (!access(fname, F_OK) && access(fname, W_OK))
			fd = -1;
	}
	if (fd != -1)
		fd = open(fname, mode, 0644);
	return (fd);
}