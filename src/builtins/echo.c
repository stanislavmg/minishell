/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:16:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 09:47:58 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_echo(char **args)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	if (!args)
		return (EXIT_FAILURE);
	while (args[i] && !ft_strcmp(args[i], "-n") && i++)
		flag = 1;
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
