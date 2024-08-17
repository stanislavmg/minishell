/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 12:45:34 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	ft_isnum(char *str)
{
	if (!str)
		return (EXIT_FAILURE);
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (EXIT_SUCCESS);
		str++;
	}
	return (EXIT_FAILURE);
}

// static int	ft_isspace(char c)
// {
// 	return (c == ' ' || c == '\t' || c == '\n'
// 		|| c == '\r' || c == '\v' || c == '\f');
// }

static long long int	ft_atol(const char *str)
{
	int				mod;
	long long int	res;
	int				i;

	i = 0;
	res = 0;
	mod = 1;
	while (isspace(str[i]))
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

int	handle_exit(char **args, t_data *msh)
{
	long long int	exit_status;

	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args)
		return (EXIT_FAILURE);
	/* !!! SEGFAULT !!! */
	// if (args[2])
	// {
	// 	ft_putendl_fd("Too many arguments", STDERR_FILENO);
	// 	return (EXIT_FAILURE);
	// }
	//free_msh_data(msh); // TODO
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
