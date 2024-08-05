/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:27:33 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 16:30:25 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static int	cd_oldpwd(t_env *env)
{
	char	*pwd;
	char	*oldpwd;

	// if !env return (0)
	pwd = get_env("PWD", env);
	oldpwd = get_env("OLDPWD", env);
	// if !pwd так как может быть unset PWD 
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
		set_env(env, "OLDPWD", ft_strdup(pwd));
		set_env(env, "PWD", ft_strdup(oldpwd));
	}
	return (EXIT_SUCCESS);
}

static int	cd_home(t_env *env)
{
	char	*home;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	home = get_env("HOME", env);
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
		set_env(env, "OLDPWD", ft_strdup(pwd));
		set_env(env, "PWD", ft_strdup(home));
	}
	free(pwd);
	return (EXIT_SUCCESS);
}

static int	cd_path(char *path, t_env *env)
{
	int		result;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		ft_putendl_fd("No such file or directory", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else
	{
		set_env(env, "OLDPWD", ft_strdup(cwd));
		set_env(env, "PWD", ft_strdup(path));
	}
	free(cwd);
	return (EXIT_SUCCESS);
}

int	handle_cd(char **args, t_env *env, int *fd)
{
	int	res;

	// лучше использовать размер массива
	// argc = size of args
	if (args[1] && args[2]) // SEGFAULT
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
