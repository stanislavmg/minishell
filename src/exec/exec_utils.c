/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/25 23:31:11 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

extern int	g_exit_code;

void	print_err(char *sender, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(sender, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	else
		perror("minishell: ");
}

void	exit_failure(char *sender, int error)
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
	{
		if (error == ERR_INIT_STAT)
			print_err(sender, "stat() fail\n");
		else if (error == ERR_EXECVE)
			print_err(sender, "execve() fail\n");
		else
			print_err(sender, NULL);
	}
	exit(exit_code);
}

static void	kill_child(t_list *ps)
{
	while (ps)
	{
		kill(*((int *)ps->data), SIGINT);
		ps = ps->next;
	}
}

void	free_minishell_data(t_data *msh)
{
	free_ast(msh->root);
	ft_lstclear(&msh->env, free_env);
	if (msh->child_ps)
		kill_child(msh->child_ps);
	ft_lstclear(&msh->child_ps, free);
	free(msh);
}

void	panic(t_data *msh)
{
	free_minishell_data(msh);
	perror("minishell: ");
	exit(EXIT_FAILURE);
}

int	get_exit_code(void)
{
	return (g_exit_code);
}

void	set_last_status(t_list *list_env, int new_value)
{
	t_env	*last_status;

	g_exit_code = new_value;
	last_status = NULL;
	if (!list_env)
		return ;
	last_status = get_env(list_env, "?");
	if (!last_status)
		return ;
	free(last_status->value);
	last_status->value = ft_itoa(new_value);
}

char	**new_env_arr(t_list *list_env)
{
	int		i;
	t_env	*var;
	int		size;
	char	**envp;
	char	*t;

	envp = NULL;
	i = 0;
	size = ft_lstsize(list_env);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	while (i < size)
	{
		var = list_env->data;
		t = ft_strjoin(var->key, "=");
		envp[i] = ft_strjoin(t, var->value);
		free(t);
		list_env = list_env->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	check_file_permission(char *fname, int mode)
{
	if (mode == O_RDONLY)
	{
		if (access(fname, F_OK) != 0)
		{
			print_err(fname, "No such file or directory\n");
			return (1);
		}
		if (access(fname, R_OK) != 0)
		{
			print_err(fname, "Permission denied\n");
			return (1);
		}
		return (0);
	}
	if ((mode & O_RDWR) || (mode & O_TRUNC))
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
