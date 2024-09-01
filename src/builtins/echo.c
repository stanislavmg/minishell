/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:16:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/01 17:47:28 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	string_of_n(char *str)
{
	int	i;

	i = 0;
	if (str[i++] != '-')
		return (0);
	while (str[i])
	{
		if (str[i++] != 'n')
			return (0);
	}
	return (1);
}

int	handle_echo(char **args)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	if (!args)
		return (EXIT_FAILURE);
	while (args[i] && string_of_n(args[i]) && i++)
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
