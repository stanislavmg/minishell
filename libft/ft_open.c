/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:05:08 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 18:05:44 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_open(char *fname, int mode)
{
	int	fd;

	fd = 0;
	if (mode == O_RDONLY)
	{
		if (access(fname, F_OK) || access(fname, R_OK))
			fd = -1;
	}
	else
	{
		if (!access(fname, F_OK) && access(fname, W_OK))
			fd = -1;
	}
	if (fd != -1)
		fd = open(fname, mode, 0644);
	return (fd);
}
