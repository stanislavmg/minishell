/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:10:30 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 17:10:49 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	fill_export_string(char *str, t_list *env)
{
	char	*s;
	int		i;
	int		j;
	t_env	*target;

	i = -1;
	j = 0;
	target = (t_env *)env -> data;
	s = target -> key;
	while (s[++i])
		str[i] = s[i];
	s = target -> value;
	if (s && ft_strlen(s) > 0)
	{
		str[i++] = '=';
		str[i++] = '"';
		while (s[j])
		str[i++] = s[j++];
		str[i++] = '"';
		str[i] = '\0';
	}
}

static void	ft_free_exports(char **exports, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(exports[i]);
		i++;
	}
}

static int	fill_string(t_list *env, t_env *target, char **exports, int i)
{
	int	addition_len;

	addition_len = (ft_strlen(target -> value) > 0) ? 4 : 0 ;
	exports[i] = malloc(sizeof(char) * (ft_strlen(target -> key)
		+ ft_strlen(target -> value) + addition_len));
	if (!exports[i])
	{
		ft_free_exports(exports, i);
		return (EXIT_FAILURE);
	}
	fill_export_string(exports[i], env);
	return (EXIT_SUCCESS);
}

static char	**get_exports(t_list *env)
{
	char	**exports;
	int		count;
	int		i;
	t_env	*target;

	i = 0;
	count = ft_env_count(env, EXPORT);
	exports = malloc(sizeof(char *) * (count + 1));
	if (!exports)
		return (NULL);
	while (i < count)
	{
		target = (t_env *) env -> content;
		if (target->attr & EXPORT)
		{
			fill_string(env, target, exports, i);
			i++;
		}
		env = env -> next;
	}
	exports[i] = NULL;
	return (exports);
}

static char	**sort_exports(char **exports)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (exports[i + 1])
	{
		j = i + 1;
		if (strcmp(exports[i], exports[j]) > 0)
		{
			temp = exports[j];
			exports[j] = exports[i];
			exports[i] = temp;
			i = 0;
		}
		else
			i++;
	}
	return (exports);
}

static void	print_export_list(t_list *env)
{
	char	**exports;
	int		i;

	i = 0;
	exports = get_exports(env);
	exports = sort_exports(exports);
	while (exports[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(exports[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	free_array(exports);
}

int	ft_arg_is_correct(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_printf("minishell: export: '%s': not a valid identifier\n", "NULL");
		return (0);
	}
	if (!((str[0] == '_') || ft_isalpha(str[0])))
	{
		ft_printf("minishell: export: '%s': not a valid identifier\n", str);
		return (0);
	}
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]))
		{
			ft_printf("minishell: export: '%s': not a valid identifier\n", str);
			return (0);
		}
		i++;
	}
	return (1);
}

int	concat_value(t_env *node, char *value, char *new_key)
{
	char	*new_value;

	if (ft_strlen(value) == 0)
		return (EXIT_SUCCESS);
	new_value = (char *)malloc(sizeof(char) * (ft_strlen(node->value)
		+ ft_strlen(value) + 1));
	if (!new_value)
	{
		free(new_key);
		return (EXIT_FAILURE);
	}
	ft_strlcpy(new_value, node->value, ft_strlen(node->value) + 1);
	ft_strlcpy(new_value + ft_strlen(node->value),
		value, ft_strlen(value) + 1);
	free(node->value);
	node->value = new_value;
	return (EXIT_SUCCESS);
}

int	concat_env_value(t_list *env, char *key, char *value)
{
	t_env	*node;
	char	*new_value;
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

int	handle_export(char **args, t_list *env)
{
	int		result;
	int		i;
	char**	arr;

	result = EXIT_SUCCESS;
	i = 1;
	if (!args || !env)
		return (EXIT_FAILURE);
	if (!args[1])
	{
		print_export_list(env);
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		if (ft_arg_is_correct(args[i]))
		{
			if (ft_strchr(args[i], '='))
			{
				arr = ft_first_split(args[i], '=');
				if (!arr)
					return (EXIT_FAILURE);
				if (arr[0][ft_strlen(arr[0]) - 1] == '+')
				{
					result = concat_env_value(env, arr[0], arr[1]);
					if (result == EXIT_FAILURE)
					{
						free_array(arr);
						return (EXIT_FAILURE);
					}
				}
				else
				{
					result = set_env_value(env, arr[0], arr[1]);
					if (result == EXIT_FAILURE)
					{
						free_array(arr);
						return (EXIT_FAILURE);
					}
				}
				free(arr);
			}
			else
				result = set_env_value(env, ft_strdup(args[i]), ft_strdup(""));
		}
		i++;
	}
	return (result);
}
