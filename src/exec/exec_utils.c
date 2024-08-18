/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/18 11:22:45 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "exit.h"
extern int g_exit_code;

void	exit_failure(char *msg, int error)
{
	if (error == CMD_NOT_FOUND)
	{
		if (ft_strchr(msg, '/'))
			ft_printf("minishell: %s: No such file or directory\n", msg);
		else
			ft_printf("minishell: %s: command not found\n", msg);
	}
	else if (error == PERM_DENIED)
		ft_printf("minishell: %s: Permission denied\n", msg);
	else
	{
		ft_printf("minishell: ");
		perror(msg);
	}
	exit(error);
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
	exit(errno);
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
	//t_env	*last_status;

	g_exit_code = new_value;
	// last_status = NULL;
	// if (!list_env)
	// 	return ;
	// last_status = get_env(list_env, "?");
	// if (!last_status)
	// 	return ;
	// free(last_status->value);
	// last_status->value = ft_itoa(new_value);
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