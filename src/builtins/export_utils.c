/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:45:21 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/24 07:49:36 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	free_arr_and_return_failure(char **arr)
{
	free_array(arr);
	return (EXIT_FAILURE);
}

int	concat_value(t_env *node, char *value, char *new_key)
{
	char	*new_value;
	int		node_value_len;

	node_value_len = (node->value == NULL) ? 0 : ft_strlen(node->value) ;
	if (ft_strlen(value) == 0)
		return (EXIT_SUCCESS);
	new_value = (char *)malloc(sizeof(char) * (node_value_len
		+ ft_strlen(value) + 1));
	if (!new_value)
	{
		free(new_key);
		return (EXIT_FAILURE);
	}
	if (node_value_len > 0)
		ft_strlcpy(new_value, node->value, node_value_len + 1);
	ft_strlcpy(new_value + node_value_len, value, ft_strlen(value) + 1);
	free(node->value);
	node->value = new_value;
	return (EXIT_SUCCESS);
}

int	concat_env_value(t_list *env, char *key, char *value)
{
	t_env	*node;
	char	*new_key;

	if (!env || !key || !value)
		return (EXIT_FAILURE);
	new_key = malloc(sizeof(char) * (ft_strlen(key)));
	if (!new_key)
		return (EXIT_FAILURE);
	ft_strlcpy(new_key, key, ft_strlen(key));
	node = get_env(env, new_key);
	if (node)
	{
		if (concat_value(node, value, new_key) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		node = new_env(new_key, value, ENV | EXPORT);
		ft_lstadd_back(&env, ft_lstnew(node));
	}
	free(key);
	return (EXIT_SUCCESS);
}
