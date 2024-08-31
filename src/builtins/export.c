/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:10:30 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 19:29:31 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	print_export_list(t_list *env)
{
	char	**exports;
	int		i;

	i = 0;
	exports = get_exports(env, i);
	i = 0;
	if (!exports)
	{
		ft_print_error("export", "", "malloc error");
		return ;
	}
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

static int	ft_arg_is_correct(char *str)
{
	int	i;

	i = 0;
	if (!str)
	{
		ft_print_error("export", "NULL", "not a valid identifier");
		return (0);
	}
	if (!((str[0] == '_') || ft_isalpha(str[0])))
	{
		ft_print_error("export", str, "not a valid identifier");
		return (0);
	}
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			ft_print_error("export", str, "not a valid identifier");
			return (0);
		}
		i++;
	}
	return (1);
}

static int	set_export_value(char **args, t_list *env, int i)
{
	int		ret;
	char	**arr;

	if (ft_strchr(args[i], '='))
	{
		arr = ft_first_split(args[i], '=');
		if (!arr)
			return (EXIT_FAILURE);
		if (arr[0][ft_strlen(arr[0]) - 1] == '+')
		{
			ret = concat_env_value(env, arr[0], arr[1]);
			if (ret == EXIT_FAILURE)
				return (free_arr_and_return_failure(arr));
		}
		else
		{
			ret = set_env_value(env, arr[0], arr[1]);
			if (ret == EXIT_FAILURE)
				return (free_arr_and_return_failure(arr));
		}
		free(arr);
	}
	else
		ret = set_env_value(env, ft_strdup(args[i]), NULL);
	return (ret);
}

int	handle_export(char **args, t_list *env)
{
	int		result;
	int		i;
	int		ret;

	result = EXIT_SUCCESS;
	ret = EXIT_SUCCESS;
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
			ret = set_export_value(args, env, i);
		else
			result = EXIT_FAILURE;
		i++;
	}
	if (ret == EXIT_FAILURE)
		result = EXIT_FAILURE;
	return (result);
}
