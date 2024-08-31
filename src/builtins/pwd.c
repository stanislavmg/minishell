/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 10:54:08 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 07:19:32 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_pwd(char **args)
{
	char	pwd[PATH_MAX];

	if (!args)
		return (EXIT_FAILURE);
	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else
	{
		ft_putendl_fd("CWD error", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
