/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/09 18:46:35 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	free_node(void *node)
{
	t_env	*env;

	env = (t_env *)node;
	free(env->key);
	free(env->value);
	free(env);
}

void	env_delete(t_list **env, char *key)
{
	t_env	*var;
	t_list	*tmp;
	t_list	*prev;

	if (!env || !key)
		return ;
	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		var = tmp->data;
		if (ft_strcmp(var->key, key) == 0)
		{
			if (!prev)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			ft_lstdelone(tmp, free_node);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

static int	ft_arg_is_correct(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_print_error("unset", "NULL", "not a valid identifier");
		return (0);
	}
	if (!((str[0] == '_') || ft_isalpha(str[0])))
	{
		ft_print_error("unset", str, "not a valid identifier");
		return (0);
	}
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_print_error("unset", str, "not a valid identifier");
			return (0);
		}
		i++;
	}
	return (1);
}

int	handle_unset(char **args, t_list **env)
{
	int	argc;
	int	i;
	int	result;

	result = EXIT_SUCCESS;
	if (!args || !env)
		return (EXIT_FAILURE);
	argc = ft_count_args(args);
	if (argc < 2)
		return (EXIT_SUCCESS);
	i = 1;
	while (args[i])
	{
		if (ft_arg_is_correct(args[i]))
			env_delete(env, args[i]);
		else
			result = EXIT_FAILURE;
		i++;
	}
	return (result);
}
