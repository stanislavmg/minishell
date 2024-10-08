/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:34:31 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 15:23:05 by amikhush         ###   ########.fr       */
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
		free(key);
		node->attr = node->attr | ENV | EXPORT;
		if (value == NULL)
			return (EXIT_SUCCESS);
		if (value == node->value)
			return (EXIT_SUCCESS);
		free(node->value);
		node -> value = value;
	}
	else
	{
		node = new_env(key, value, ENV | EXPORT);
		ft_lstadd_back(&env, ft_lstnew(node));
	}
	return (EXIT_SUCCESS);
}

int	ft_env_count(t_list *env, int attr)
{
	int		count;
	t_env	*target;

	count = 0;
	while (env)
	{
		target = (t_env *) env -> data;
		if (target -> attr & attr)
			count++;
		env = env -> next;
	}
	return (count);
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
			if (target -> value != NULL)
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
