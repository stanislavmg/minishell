/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:16:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 16:30:53 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

int	handele_echo(char **args, t_env *env, int *fd)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	// проверки на null
	while (ft_strcmp(args[i], "-n") == 0)// SEGFAULT
	{
		flag = 1;
		i++;
	}
	while (args[i])
	{
		write(fd[1], args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(fd[1], " ", 1);
		i++;
	}
	if (!flag)
		write(fd[1], "\n", 1);
	return (EXIT_SUCCESS);
}
