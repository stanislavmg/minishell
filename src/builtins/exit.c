/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/22 16:28:25 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
extern int g_exit_code;

static int	ft_isnum(char *str)
{
	if (!str)
		return (EXIT_FAILURE);
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		str++;
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

int	handle_exit(char **args, t_data *msh)
{
	long long int	exit_status;
	int	argc;

	if (!args)
		return (EXIT_FAILURE);
	argc = ft_count_args(args);
	if (argc > 2)
	{
		ft_print_error("exit", "", "too many arguments");
		return (EXIT_FAILURE);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (check_arg(args, argc) != 0)
	{
 		free_minishell_data(msh);
		exit(255);
	}
	if (args[1])
	{
		exit_status = ft_atol(args[1]);
		if (exit_status > LLONG_MAX || exit_status < LLONG_MIN)
		{
			ft_print_error("exit", args[1], "numeric argument required");
			exit(255);
		}
		while (exit_status >= 256)
			exit_status %= 256;
 		free_minishell_data(msh);
		exit(exit_status);
	}
	else
	{
 		free_minishell_data(msh);
		exit(g_exit_code);
	}
	return (EXIT_SUCCESS);
}
