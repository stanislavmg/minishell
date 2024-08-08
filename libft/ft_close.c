# include "libft.h"

int	ft_close(int fd)
{
	/* If fd isn't std stream */
	if (fd > 3)
	{
		if (close(fd))
			return (-1);
	}
	return (0);
}