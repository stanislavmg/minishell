/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/20 15:09:27 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
extern int g_exit_code;

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
	int exit_code;

	exit_code = 0;
	if (error >= CMD_NOT_FOUND)
	{	
		if (error == ERR_IS_DIR && ft_strchr(sender, '/'))
		{
			print_err(sender, "is a directory\n");
			exit_code = PERM_DENIED;
		}
		else if (ft_strchr(sender, '/'))
		{
			print_err(sender, "No such file or directory\n");
			exit_code = CMD_NOT_FOUND;
		}
		else
		{
			print_err(sender, "command not found\n");
			exit_code = CMD_NOT_FOUND;
		}
	}
	else if (error == PERM_DENIED)
	{
		print_err(sender, "Permission denied\n");
		exit_code = PERM_DENIED;
	}
	else if (error == ERR_INIT_STAT)
	{
		print_err(sender, "stat() fail\n");
		exit_code = EXIT_FAILURE;
	}
	else if (error == ERR_EXECVE)
	{
		print_err(sender, "execve() fail\n");
		exit_code = EXIT_FAILURE;
	}
	else
	{
		print_err(sender, NULL);
		exit_code = EXIT_FAILURE;
	}
	if (exit_code)
		exit(exit_code);
}

void	free_minishell_data(t_data *msh)
{
	free_ast(msh->root);
	ft_lstclear(&msh->env, free_env);
	free(msh);
}

void	panic(t_data *msh)
{
	free_minishell_data(msh);
	perror("minishell: ");
	exit(EXIT_FAILURE);
}

int	get_last_status(t_list *list_env)
{
	// t_env	*last_status;

	// if (!list_env)
	// 	return (1);
	// last_status = get_env(list_env, "?");
	// if (!last_status)
	// 	return (1);
	// return (ft_atoi(last_status->value));
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

pid_t	ft_fork(void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: ");
		return (-1);
	}
	return (pid);
}

int	check_file_permission(char *fname, int mode)
{
	if (mode == O_RDONLY)
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
	}
	else if (!access(fname, F_OK) && access(fname, W_OK))
	{
		print_err(fname, "Permission denied\n");
		return (1);
	}
	return (0);
}