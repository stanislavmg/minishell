/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 07:38:04 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/25 07:40:14 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	cd_oldpwd(t_list *env)
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
	if (chdir(oldpwd) != 0)
	{
		perror("chdir error");
		free(pwd);
		return (EXIT_FAILURE);
	}
	else
		set_pwd_vals(env, pwd, oldpwd);
	free(pwd);
	return (EXIT_SUCCESS);
}

int	cd_home(t_list *env)
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

int	cd_path(char *path, t_list *env)
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