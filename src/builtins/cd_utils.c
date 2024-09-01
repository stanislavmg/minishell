/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 07:38:04 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/01 14:48:49 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	set_pwd_vals(t_list *env, char *pwd, char *oldpwd)
{
	char	*tmp;

	tmp = ft_strdup(pwd);
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	set_env_value(env, ft_strdup("PWD"), ft_strdup(oldpwd));
	set_env_value(env, ft_strdup("OLDPWD"), tmp);
}

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
		return (EXIT_FAILURE);
	}
	if (chdir(oldpwd) != 0)
	{
		ft_print_error("cd", oldpwd, "No such file or directory");
		return (EXIT_FAILURE);
	}
	set_pwd_vals(env, pwd, oldpwd);
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
		ft_print_error("cd", home, "No such file or directory");
		free(pwd);
		return (EXIT_FAILURE);
	}
	set_env_value(env, ft_strdup("OLDPWD"), pwd);
	set_env_value(env, ft_strdup("PWD"), ft_strdup(home));
	return (EXIT_SUCCESS);
}

int	cd_path(char *path, t_list *env)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
<<<<<<< HEAD
	if (cwd)
		set_env_value(env, ft_strdup("OLDPWD"), cwd);
	else
	{
		ft_print_error("cd", "", CWD_RETRIEVING_ERROR);
		return (EXIT_SUCCESS);
=======
	if (!cwd)
	{
		perror("minishell: ");
		return (1);
>>>>>>> d00b845 (FIX: normi, replace lexer func in lexer.h)
	}
	if (chdir(path) != 0)
	{
		ft_print_error("cd", path, "No such file or directory");
		return (EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_print_error("cd", "", CWD_RETRIEVING_ERROR);
		return (EXIT_SUCCESS);
	}
	set_env_value(env, ft_strdup("PWD"), pwd);
	return (EXIT_SUCCESS);
}
