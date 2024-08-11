/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/11 17:37:24 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exit_failure(char *msg, int error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == CMD_NOT_FOUND)
	{
		if (ft_strchr(msg, '/'))
			ft_printf("minishell: %s: No such file or directory\n", msg);
		else
			ft_printf("minishell: %s: command not found\n", msg);
	}
	else if (error == PERM_DENIED)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd(":", STDERR_FILENO);
		ft_putstr_fd(" Permission denied\n", STDERR_FILENO);
	}
	else
		perror(msg);
	exit(error);
}

int	get_last_status(t_list *list_env)
{
	t_env	*last_status;

	if (!list_env)
		return (1);
	last_status = get_env(list_env, "?");
	if (!last_status)
		return (1);
	return (ft_atoi(last_status->value));
}

void	set_last_status(t_list *list_env, int new_value)
{
	t_env	*last_status;

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