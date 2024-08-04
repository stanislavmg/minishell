/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:34:31 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:16:42 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

char	*get_env(char *key, t_env *env)
{
	t_env	*node;

	node = list_search(env, key);
	if (!node)
		return (NULL);
	return (node -> value);
}

int	set_env(t_env *env, char *key, char *value)
{
	t_env	*node;

	node = list_search(env, key);
	if (node)
	{
		if (ft_strlen(value) == 0)
			return (0);
		node -> value = value;
	}
	else
	{
		node = list_new(key, value);
		list_add(&env, node);
	}
	return (EXIT_SUCCESS);
}

int	handle_env(char **args, t_env *env, int *fd)
{
	while (env)
	{
		if (ft_strlen(env -> value) > 0)
		{
			ft_putstr_fd(env -> key, fd[1]);
			ft_putstr_fd("=", fd[1]);
			ft_putstr_fd(env -> value, fd[1]);
			ft_putstr_fd("\n", fd[1]);
		}
		env = env -> next;
	}
	return (EXIT_SUCCESS);
}
