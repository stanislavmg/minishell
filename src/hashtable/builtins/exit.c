/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:16:52 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static int	ft_isnum(char *str)
{
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (EXIT_SUCCESS);
		str++;
	}
	return (EXIT_FAILURE);
}

static long long int	ft_atol(const char *str)
{
	int				mod;
	long long int	res;
	int				i;

	i = 0;
	res = 0;
	mod = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			mod *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	res *= mod;
	return (res);
}

int	handle_exit(char **args, t_env *env, int *fd)
{
	long long int	exit_status;

	if (args[2])
	{
		ft_putendl_fd("Too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (args[1])
	{
		if (!ft_isnum(args[1]))
		{
			ft_putendl_fd("numeric argument required", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		if (ft_strlen(args[1]) > 19)
		{
			ft_putendl_fd("Incorrect argument", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		exit_status = ft_atol(args[1]);
		if (exit_status > INT_MAX || exit_status < INT_MIN)
		{
			ft_putendl_fd("Incorrect argument", STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		while (exit_status >= 256)
			exit_status %= 256;
		exit(exit_status);
	}
	else
		exit(0);
	return (EXIT_SUCCESS);
}
