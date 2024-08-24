/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_print.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 07:40:39 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/24 08:21:23 by amikhush         ###   ########.fr       */
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
	target = (t_env *)env -> content;
	s = target -> key;
	while (s[++i])
		str[i] = s[i];
	s = target -> value;
	if (s)
	{
		str[i++] = '=';
		str[i++] = '"';
		while (s[j])
		str[i++] = s[j++];
		str[i++] = '"';
		str[i] = '\0';
	}
	else
		str[i++] = '\0';
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
	int	value_len;

	if (target -> value == NULL)
		addition_len = 1;
	else
		addition_len = 4;
	if (target -> value == NULL)
		value_len = 0;
	else
		value_len = ft_strlen(target -> value);
	exports[i] = malloc(sizeof(char) * (ft_strlen(target -> key)
		+ value_len + addition_len));
	if (!exports[i])
	{
		ft_free_exports(exports, i);
		return (EXIT_FAILURE);
	}
	fill_export_string(exports[i], env);
	return (EXIT_SUCCESS);
}

char	**get_exports(t_list *env)
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

char	**sort_exports(char **exports)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (exports[i + 1])
	{
		j = i + 1;
		if (ft_strcmp(exports[i], exports[j]) > 0)
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
