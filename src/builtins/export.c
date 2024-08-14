/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:10:30 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/14 16:07:08 by sgoremyk         ###   ########.fr       */
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
	count = ft_lstsize(env);
	exports = malloc(sizeof(char *) * (count + 1));
	if (!exports)
		return (NULL);
	while (i < count)
	{
		target = (t_env *) env -> data;
		exports[i] = malloc(sizeof(char) * (ft_strlen(target -> key)
			+ ft_strlen(target -> value) + 4));
		if (!exports[i])
		{
			ft_free_exports(exports, i);
			return (NULL);
		}
		fill_export_string(exports[i], env);
		i++;
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

int	handle_export(char **args, t_list *env)
{
	int	result;

	result = 0;
	if (!args || !env)
		return (EXIT_FAILURE);
	if (!args[1])
		print_export_list(env);
	else
	{
		if (args[1] && args[3])	
			result = set_env_value(env, ft_strdup(args[1]), ft_strdup(args[3]));
		else if (args[1] && !args[3])
		{
			result = set_env_value(env, ft_strdup(args[1]), ft_strdup(""));
		}
		else
			ft_putendl_fd("Incorrect input", STDERR_FILENO);
	}
	return (result);
}
