/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 11:51:28 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

extern int	g_exit_code;
static int	check_input_file(const char *fname);

void	print_err(const char *sender, const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(sender, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	else
		perror("minishell: ");
}

void	exit_failure(const char *sender, int error)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (error == CMD_NOT_FOUND)
	{
		if (ft_strchr(sender, '/'))
			print_err(sender, "No such file or directory\n");
		else
			print_err(sender, "command not found\n");
		exit_code = CMD_NOT_FOUND;
	}
	else if (error == ERR_IS_DIR || error == PERM_DENIED)
	{
		if (error == ERR_IS_DIR)
			print_err(sender, "is a directory\n");
		else if (error == PERM_DENIED)
			print_err(sender, "Permission denied\n");
		exit_code = PERM_DENIED;
	}
	else
		print_err(sender, NULL);
	exit(exit_code);
}

void	panic(t_data *msh)
{
	kill_child(msh->child_ps);
	free_minishell_data(msh);
	perror("minishell: ");
	exit(EXIT_FAILURE);
}

int	check_file_permission(char *fname, int mode)
{
	if (mode == O_RDONLY)
		return (check_input_file(fname));
	else if ((mode & O_RDWR) || (mode & O_TRUNC))
	{
		if (access(fname, F_OK) == 0)
		{
			if (access(fname, W_OK) != 0)
			{
				print_err(fname, "Permission denied\n");
				return (1);
			}
		}
		else if (!(mode & O_CREAT))
		{
			print_err(fname, "No such file or directory\n");
			return (1);
		}
	}
	return (0);
}

static int	check_input_file(const char *fname)
{
	if (access(fname, F_OK))
	{
		print_err(fname, "No such file or directory\n");
		return (1);
	}
	if (access(fname, R_OK))
	{
		print_err(fname, "Permission denied\n");
		return (1);
	}
	return (0);
}
