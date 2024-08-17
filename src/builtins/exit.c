/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 13:20:32 by amikhush         ###   ########.fr       */
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

static int	check_arg(char **args)
{
	if (args[2])
	{
		ft_putendl_fd("Too many arguments", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (!ft_isnum(args[1]))
	{
		printf("minishell: exit: '%s': numeric argument required\n", args[1]);
		return (EXIT_FAILURE);
	}
	if (ft_strlen(args[1]) > 19) // 
	{
		printf("minishell: exit: '%s': numeric argument required\n", args[1]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	handle_exit(char **args, t_data *msh)
{
	long long int	exit_status;

	if (!args)
		return (EXIT_FAILURE);
  free_minishell_data(msh);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (check_arg(args) != 0)
		exit(255);
	if (args[1])
	{
		exit_status = ft_atol(args[1]);
		if (exit_status > LLONG_MAX || exit_status < LLONG_MIN)
		{
			printf("minishell: exit: '%s': numeric argument required\n", args[1]);
			exit(255);
		}
		while (exit_status >= 256)
			exit_status %= 256;
		exit(exit_status);
	}
	else
		exit(0);
	return (EXIT_SUCCESS);
}
