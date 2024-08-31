/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:48:35 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 18:53:07 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_command(char **args, t_data *msh)
{
	int	result;

	if (!args || !msh)
		return (EXIT_FAILURE);
	if (strcmp(args[0], "cd") == 0)
		result = handle_cd(args, msh->env);
	else if (strcmp(args[0], "env") == 0)
		result = handle_env(args, msh->env);
	else if (strcmp(args[0], "pwd") == 0)
		result = handle_pwd(args);
	else if (strcmp(args[0], "export") == 0)
		result = handle_export(args, msh->env);
	else if (strcmp(args[0], "echo") == 0)
		result = handle_echo(args);
	else if (strcmp(args[0], "unset") == 0)
		result = handle_unset(args, msh->env);
	else if (strcmp(args[0], "exit") == 0)
		result = handle_exit(args, msh);
	else
		return (0);
	return (result);
}

void	ft_print_error(char *command, char *str, char *error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command != NULL)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (str != NULL && ft_strlen(str) > 0)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(error, STDERR_FILENO);
}

int	ft_count_args(char **args)
{
	int	count;

	count = 0;
	while (*args)
	{
		count++;
		args++;
	}
	return (count);
}
