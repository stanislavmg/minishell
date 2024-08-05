/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:10:30 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 17:50:11 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static void	fill_export_string(char *str, t_env *env)
{
	char	*s;
	int		i;
	int		j;

	i = -1;
	j = 0;
	s = env -> key;
	while (s[++i])
		str[i] = s[i];
	s = env -> value;
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

static char	**get_exports(t_env *env)
{
	char	**exports;
	int		count;
	int		i;

	i = 0;
	count = ft_list_size(env);
	exports = malloc(sizeof(char *) * (count + 1));
	// нет проверки на !export
	while (i < count)
	{
		// не совсем понял зачем выделять память под строки
		exports[i] = malloc(sizeof(char) * (ft_strlen(env->key) + ft_strlen(env->value) + 4));
		fill_export_string(exports[i], env);
		env = env->next;
		i++;
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

static void	print_export_list(t_env *env, int *fd)
{
	char	**exports;
	int		i;

	i = 0;
	exports = get_exports(env);
	exports = sort_exports(exports);
	while (exports[i])
	{
		ft_putstr_fd("declare -x ", fd[1]);
		ft_putstr_fd(exports[i], fd[1]);
		ft_putstr_fd("\n", fd[1]);
		i++;
	}
	free_array(exports);
}

int	handle_export(char **args, t_env *env, int *fd)
{
	int	result;

	result = 0;
	if (!args[1])
		print_export_list(env, fd);
	else
	{
		if (args[1] && args[3])	
			result = set_env(env, ft_strdup(args[1]), ft_strdup(args[3]));
		else if (args[1] && !args[3])
		{
			result = set_env(env, ft_strdup(args[1]), ft_strdup(""));
		}
		else
			ft_putendl_fd("Incorrect input", STDERR_FILENO);
	}
	return (result);
}
