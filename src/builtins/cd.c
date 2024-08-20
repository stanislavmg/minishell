/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:27:33 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/20 20:01:35 by amikhush         ###   ########.fr       */
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
	oldpwd = get_env_value("OLDPWD", env);
	if (!oldpwd)
	{
		ft_print_error("cd", "", "OLDPWD not set");
		free(pwd);
		return (EXIT_FAILURE);
	}
	/* часть кода повторяется в 3 функциях, можно вынести в отдельную 
	например exit_failure & swap_value*/
	if (chdir(oldpwd) != 0)
	{
		perror("chdir error");
		free(pwd);
		return (EXIT_FAILURE);
	}
	else
	{
		ft_putendl_fd(oldpwd, STDOUT_FILENO);
		set_env_value(env, "PWD", ft_strdup(oldpwd));
		set_env_value(env, "OLDPWD", ft_strdup(pwd));
	}
	free(pwd);
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
		ft_print_error("cd", "", "HOME not set");
		free(pwd);
		return (EXIT_FAILURE);
	}
	if (chdir(home) != 0)
	{
		perror("minishell: chdir error: ");
		free(pwd);
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
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		ft_print_error("cd", path, "No such file or directory");
		free(cwd);
		return (EXIT_FAILURE);
	}
	else
	{
		pwd = getcwd(NULL, 0);
		set_env_value(env, "OLDPWD", ft_strdup(cwd));
		set_env_value(env, "PWD", ft_strdup(pwd));
		free(pwd);
	}
	free(cwd);
	return (EXIT_SUCCESS);
}

int	handle_cd(char **args, t_list *env)
{
	int	res;
	int	argc;

	argc = ft_count_args(args);
	if (!args || !env || argc == 0)
		return (EXIT_FAILURE);
	if (argc > 2)
	{
		ft_print_error("cd", "", "Too many arguments");
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		res = cd_home(env);
	else if (strcmp(args[1], "-") == 0)
		res = cd_oldpwd(env);
	else
		res = cd_path(args[1], env);	
	return (res);
}
