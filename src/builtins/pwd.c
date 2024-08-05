/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:54:08 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 17:39:45 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

int	handle_pwd(char **args, t_env *env, int *fd)
{
	char	pwd[PATH_MAX];
	// зачем тут env
	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_putendl_fd(pwd, fd[1]);
	else
	{
		ft_putendl_fd("CWD error", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
