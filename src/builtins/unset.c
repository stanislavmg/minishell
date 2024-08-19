/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/19 20:09:20 by amikhush         ###   ########.fr       */
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
	t_list	*head;

	head = *env;
	if (!key)
		return ;
	prev = NULL;
	while (*env)
	{
		node = (t_env *)(*env)->data;
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
	*env = head;
}

static int	ft_arg_is_correct(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		printf("minishell: unset: 'NULL': not a valid identifier\n");
		return (0);
	}
	if (!((str[0] == '_') || ft_isalpha(str[0])))
	{
		printf("minishell: unset: '%s': not a valid identifier\n", str);
		return (0);
	}
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			printf("minishell: unset: '%s': not a valid identifier\n", str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	handle_unset(char **args, t_list *env)
{
	int	argc;
	int	i;

	if (!args || !env)
		return (EXIT_FAILURE);
	argc = ft_count_args(args);
	if (argc < 2)
		return (EXIT_SUCCESS);
	i = 1;
	while (args[i])
	{
		if (ft_arg_is_correct(args[i]))
			env_delete(&env, args[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
