/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:34:31 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/14 16:07:08 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	ft_content_del(void *env)
{
	free(((t_env *)env)->key);
	free(((t_env *)env)->value);
}

void	env_delete(t_list *env, char *key)
{
	t_env	*node;
	t_list	*tmp;

	while (env)
	{
		node = get_env(tmp, key);
		if (ft_strcmp(node -> key, key) == 0)
		{
			tmp = env -> next;
			ft_lstdelone(env, &ft_content_del);
			free(env);
			env = tmp;
		}
		env = env -> next;
	}
}

char	*get_env_value(char *key, t_list *env)
{
	t_env	*node;

	node = get_env(env, key);
	if (!node)
		return (NULL);
	return (node -> value);
}

int	set_env_value(t_list *env, char *key, char *value)
{
	t_env	*node;

	node = get_env(env, key);
	if (node)
	{
		if (ft_strlen(value) == 0)
			return (EXIT_SUCCESS);
		node -> value = value;
	}
	else
	{
		node = new_env(key, value, ENV);
		ft_lstadd_back(&env, ft_lstnew(node));
	}
	return (EXIT_SUCCESS);
}

int	handle_env(char **args, t_list *env)
{
	t_env	*target;

	if (!args || !env)
		return (EXIT_FAILURE);
	while (env)
	{
		target = (t_env *)env -> data;
		if (ft_strlen(target -> value) > 0)
		{
			ft_putstr_fd(target -> key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(target -> value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env -> next;
	}
	return (EXIT_SUCCESS);
}
