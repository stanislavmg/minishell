/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 08:43:02 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	free_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

void	env_delete(t_list **env, char *key)
{
	t_env	*node;
	t_list	*tmp;
	t_list	*prev;

	if (!key)
		return ;
	prev = NULL;
	while (*env)
	{
		node = (t_env *)(*env)->content;
		if (ft_strcmp(node->key, key) == 0)
		{
			tmp = (*env)->next;
			free_node(node);
			if (prev)
				prev->next = tmp;
			else
				*env = tmp;
		}
		prev = *env;
		*env = (*env) -> next;
	}
}

int	handle_unset(char **args, t_list *env)
{
	t_env	*node;

	if (!args || !env)
		return (EXIT_FAILURE);
	if (!args[1])
		return (EXIT_SUCCESS);
	env_delete(&env, args[1]);
	return (EXIT_SUCCESS);
}
