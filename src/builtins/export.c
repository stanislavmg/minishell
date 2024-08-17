/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:10:30 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/17 09:52:30 by amikhush         ###   ########.fr       */
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
		target = (t_env *) env -> data;
		if (target->attr & EXPORT)
		{
			exports[i] = malloc(sizeof(char) * (ft_strlen(target -> key)
				+ ft_strlen(target -> value) + 4));
			if (!exports[i])
			{
				ft_free_exports(exports, i);
				return (NULL);
			}
			fill_export_string(exports[i], env);
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
				arr = ft_split(args[i], '=');
				if (!arr)
					return (EXIT_FAILURE);
				result = set_env_value(env, arr[0], arr[1]);
				free(arr);
			}
			else
				result = set_env_value(env, ft_strdup(args[i]), ft_strdup(""));
		}
		i++;
	}
	return (result);
}
