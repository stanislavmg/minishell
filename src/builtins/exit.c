/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/24 08:36:36 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
extern int g_exit_code;

static long long int	ft_atol(const char *str)
{
	int				mod;
	long long int	res;
	int				i;

	i = 0;
	res = 0;
	mod = 1;
	while (ft_isspace(str[i]))
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

static int	check_arg(char **args, int argc)
{
	if (!args || argc == 0)
		return (EXIT_FAILURE);
	if (argc > 1 && !ft_isnum(args[1]))
	{
		ft_print_error("exit", args[1], "numeric argument required");
		return (EXIT_FAILURE);
	}
	if (argc > 1 && ft_strlen(args[1]) > 19)
	{
		ft_print_error("exit", args[1], "numeric argument required");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static long long int	calc_exit_status(char *arg)
{
	long long int	exit_status;

	exit_status = ft_atol(arg);
	if (exit_status > LLONG_MAX || exit_status < LLONG_MIN)
	{
		ft_print_error("exit", arg, "numeric argument required");
		return (255);
	}
	while (exit_status >= 256)
		exit_status %= 256;
	return (exit_status);
}

static int	check_and_return_exit(char **args, int argc)
{
	long long int	exit_status;

	if (check_arg(args, argc) != 0)
		return (255);
	if (args[1])
	{
		exit_status = calc_exit_status(args[1]);
		return (exit_status);
	}
	else
		return (g_exit_code);
}

int	handle_exit(char **args, t_data *msh)
{
	int				argc;
	int				result;

	if (!args)
		return (EXIT_FAILURE);
	argc = ft_count_args(args);
	if (argc > 2)
	{
		ft_print_error("exit", "", "too many arguments");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	result = check_and_return_exit(args, argc);
	free_minishell_data(msh);
	exit(result);
	return (EXIT_SUCCESS);
}
