/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/builtins/pwd.c
/*   Created: 2024/07/16 10:54:08 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/13 19:02:52 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_pwd(char **args)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) != NULL)
		ft_putendl_fd(pwd, STDERR_FILENO);
	else
	{
		ft_putendl_fd("CWD error", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
=======
/*   Created: 2024/08/05 12:30:02 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/10 21:36:41 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define PERM_DENIED	126
# define CMD_NOT_FOUND	127 		// zsh: command not found: cmd_name
# define INVALID_ARG_FOR_EXIT 128	// Invalid argument to exit
# define FT_SIGINT	130 			// Ctrl+C

# define  ERR_MEM    	101
# define  ERR_TOKEN   	102
# define  ERR_QUOTE   	103
# define  ERR_SYNTAX  	104

#endif
>>>>>>> origin/parser:include/error.h
