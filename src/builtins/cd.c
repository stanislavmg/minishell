/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:27:33 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/01 15:24:02 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	fill_newpath(char *new_path, char *path, char *home)
{
	while (*home)
	{
		*new_path = *home;
		home++;
		new_path++;
	}
	while (*path)
	{
		if (*path == '~')
			path++;
		else
		{
			*new_path = *path;
			path++;
			new_path++;
		}
	}
	*new_path = 0;
}

static int	cd_tilda(char *path, t_list *env)
{
	char	*new_path;
	char	*home;
	int		res;

	home = get_env_value("HOME", env);
	if (!home)
	{
		ft_print_error("cd", "", "HOME not set");
		return (EXIT_FAILURE);
	}
	new_path = malloc(sizeof(char) * (ft_strlen(home) + ft_strlen(path)));
	if (!new_path)
	{
		ft_print_error("cd", "", "malloc error");
		return (EXIT_FAILURE);
	}
	fill_newpath(new_path, path, home);
	res = cd_path(new_path, env);
	free(new_path);
	return (res);
}

int	handle_cd(char **args, t_list *env)
{
	int	res;
	int	argc;

	argc = ft_count_args(args);
	if (!args || !env || argc == 0)
		return (EXIT_FAILURE);
	if (argc == 1)
		res = cd_home(env);
	else if (ft_strcmp(args[1], "-") == 0)
		res = cd_oldpwd(env);
	else if (ft_strncmp(args[1], "~", 1) == 0)
		res = cd_tilda(args[1], env);
	else
		res = cd_path(args[1], env);
	return (res);
}
