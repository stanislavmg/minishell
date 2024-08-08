/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 04:49:46 by amikhush          #+#    #+#             */
/*   Updated: 2024/07/23 10:58:36 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_env	*new_env(char *key, char *value, int attr)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = key;
	env->value = value;
	env->attr = attr;
	return (env);
}

void	free_env(void *env)
{
	free(((t_env *)env)->key);
	free(((t_env *)env)->value);
	free(env);
}

t_env	*get_env(t_list *list_env, char *key)
{
	t_env *target;

	while (list_env)
	{
		target = (t_env *)list_env->data;
		if (strcmp(target->key, key) == 0)
			return (target);
		list_env = list_env->next;
	}
	return (NULL);
}