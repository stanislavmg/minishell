/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 04:49:46 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 15:24:04 by sgoremyk         ###   ########.fr       */
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
	t_env	*target;

	while (list_env)
	{
		target = (t_env *)list_env->data;
		if (ft_strcmp(target->key, key) == 0)
			return (target);
		list_env = list_env->next;
	}
	return (NULL);
}

char	*get_var_name(const char *token_word)
{
	char	*var_name;
	char	*end_var_name;
	size_t	len_var_name;

	var_name = NULL;
	end_var_name = ft_strchr(token_word, '=');
	if (!end_var_name)
		return (NULL);
	len_var_name = end_var_name - token_word;
	var_name = (char *)malloc(sizeof(char) * len_var_name + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, token_word, len_var_name + 1);
	return (var_name);
}

char	*get_var_value(const char *token_word)
{
	char	*var_value;
	char	*start_var_value;
	size_t	len_var_value;

	var_value = NULL;
	start_var_value = ft_strchr(token_word, '=') + 1;
	len_var_value = ft_strlen(start_var_value);
	var_value = (char *)malloc(sizeof(char) * len_var_value + 1);
	if (!var_value)
		return (NULL);
	ft_strlcpy(var_value, start_var_value, len_var_value + 1);
	return (var_value);
}
