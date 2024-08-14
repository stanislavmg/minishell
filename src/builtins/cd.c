/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:27:33 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/14 17:16:51 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	cd_oldpwd(t_list *env)
{
	char	*pwd;
	char	*oldpwd;

	if (!env)
		return (0);
	pwd = get_env_value("PWD", env);
	if (!pwd)
	{
		ft_putendl_fd("PWD is not set", 2);
		return (EXIT_FAILURE);
	}
	oldpwd = get_env_value("OLDPWD", env);
	if (!oldpwd)
	{
		ft_putendl_fd("OLDPWD is not set", 2);
		return (EXIT_FAILURE);
	}
	/* часть кода повторяется в 3 функциях, можно вынести в отдельную 
	например exit_failure & swap_value*/
	if (chdir(oldpwd) != 0)
	{
		perror("chdir error");
		return (EXIT_FAILURE);
	}
	else
	{
		set_env_value(env, "OLDPWD", ft_strdup(pwd));
		set_env_value(env, "PWD", ft_strdup(oldpwd));
	}
	return (EXIT_SUCCESS);
}

static int	cd_home(t_list *env)
{
	char	*home;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	home = get_env_value("HOME", env);
	if (!home)
	{
		ft_putendl_fd("HOME catalog not found", 2);
		return (EXIT_FAILURE);
	}
	if (chdir(home) != 0)
	{
		perror("chdir error");
		return (EXIT_FAILURE);
	}
	else
	{
		set_env_value(env, "OLDPWD", ft_strdup(pwd));
		set_env_value(env, "PWD", ft_strdup(home));
	}
	free(pwd);
	return (EXIT_SUCCESS);
}

static int	cd_path(char *path, t_list *env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
	{
		set_env_value(env, "OLDPWD", ft_strdup(cwd));
		set_env_value(env, "PWD", ft_strdup(path));
	}
	free(cwd);
	return (EXIT_SUCCESS);
}

int	handle_cd(char **args, t_list *env)
{
	int	res;

	if (!args || !env)
		return (EXIT_FAILURE);
	if (args[1] && args[2])
	{
		ft_putendl_fd("Too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (!args[1])
		res = cd_home(env);
	else if (strcmp(args[1], "-") == 0)
		res = cd_oldpwd(env);
	else
		res = cd_path(args[1], env);	
	return (res);
}
