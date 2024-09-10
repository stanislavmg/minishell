/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 07:10:50 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/10 20:14:56 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

extern int	g_exit_code;

void	check_llong_max(unsigned long long res, int mod, char *str, t_data *msh)
{
	if ((res > (unsigned long long)LLONG_MAX && mod == 1)
		|| (res > ((unsigned long long)LLONG_MIN) && mod == -1))
	{
		ft_print_error("exit", str, "numeric argument required");
		free_minishell_data(msh);
		exit(255);
	}
}

int	ft_atol(char *str, t_data *msh)
{
	int					mod;
	unsigned long long	res;
	int					i;

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
	check_llong_max(res, mod, str, msh);
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
	if (argc > 1 && ft_strlen(args[1]) > 20)
	{
		ft_print_error("exit", args[1], "numeric argument required");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	check_and_return_exit(char **args, int argc, t_data *msh)
{
	int	exit_status;

	if (check_arg(args, argc) != 0)
	{
		free_minishell_data(msh);
		exit(255);
	}
	if (args[1])
	{
		exit_status = calc_exit_status(args[1], msh);
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
	ft_putendl_fd("exit", STDOUT_FILENO);
	result = check_and_return_exit(args, argc, msh);
	if (argc > 2)
	{
		ft_print_error("exit", "", "too many arguments");
		return (EXIT_FAILURE);
	}
	free_minishell_data(msh);
	exit(result);
	return (EXIT_SUCCESS);
}
