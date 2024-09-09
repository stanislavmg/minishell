/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:32 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 21:00:46 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

int	is_logic_operator(e_token type)
{
	return (type == OR || type == AND || type == SEMICOLON);
}

static int	count_export_vars(t_list *env)
{
	int		size;
	t_env	*var;

	size = 0;
	while (env)
	{
		var = (t_env *)env->data;
		if (var->attr & EXPORT)
			size++;
		env = env->next;
	}
	return (size);
}

char	**new_env_arr(t_list *list_env)
{
	int		i;
	char	*t;
	t_env	*var;
	char	**envp;

	if (!list_env)
		return (NULL);
	i = 0;
	envp = (char **)malloc(sizeof(char *) * (count_export_vars(list_env) + 1));
	if (!envp)
		return (NULL);
	while (list_env)
	{
		var = list_env->data;
		if (var->attr & EXPORT)
		{
			t = ft_strjoin(var->key, "=");
			envp[i] = ft_strjoin(t, var->value);
			free(t);
			i++;
		}
			list_env = list_env->next;
	}
	envp[i] = NULL;
	return (envp);
}
