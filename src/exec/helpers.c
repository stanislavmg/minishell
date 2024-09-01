/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:32 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 11:51:35 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	else if (strcmp(cmd, "env") == 0)
		return (1);
	else if (strcmp(cmd, "pwd") == 0)
		return (1);
	else if (strcmp(cmd, "export") == 0)
		return (1);
	else if (strcmp(cmd, "echo") == 0)
		return (1);
	else if (strcmp(cmd, "unset") == 0)
		return (1);
	else if (strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

int	is_logic_operator(e_token type)
{
	return (type == OR || type == AND || type == SEMICOLON);
}

int	fd_is_pipe(int fd)
{
	struct stat	file_info;

	if (fstat(fd, &file_info) == -1)
		return (-1);
	if (S_ISFIFO(file_info.st_mode))
		return (1);
	return (0);
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
