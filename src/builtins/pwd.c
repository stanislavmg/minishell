/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:54:08 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/01 11:33:24 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_pwd(char **args, t_list *env)
{
	char	pwd[PATH_MAX];
	char	*pwd_env;

	if (!args)
		return (EXIT_FAILURE);
	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else
	{
		pwd_env = get_env_value("PWD", env);
		if (pwd_env)
			ft_putendl_fd(pwd_env, STDOUT_FILENO);
		else
		{
			ft_print_error("cd", "", CWD_RETRIEVING_ERROR);
			return (EXIT_SUCCESS);
		}	
	}
	return (EXIT_SUCCESS);
}
