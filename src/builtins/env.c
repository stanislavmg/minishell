/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:34:31 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 08:43:49 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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
		node = new_env(key, value, ENV | EXPORT);
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
		if (target -> attr & ENV)
		{
			if (ft_strlen(target -> value) > 0)
			{
				ft_putstr_fd(target -> key, STDOUT_FILENO);
				ft_putstr_fd("=", STDOUT_FILENO);
				ft_putstr_fd(target -> value, STDOUT_FILENO);
				ft_putstr_fd("\n", STDOUT_FILENO);
			}
		}
		env = env -> next;
	}
	return (EXIT_SUCCESS);
}
