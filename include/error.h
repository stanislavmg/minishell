/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:02 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/20 14:58:33 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define PERM_DENIED	126
# define CMD_NOT_FOUND	127			// zsh: command not found: cmd_name
# define ERR_IS_DIR		129

# define INVALID_ARG_FOR_EXIT 128	// Invalid argument to exit
# define FT_SIGINT	130 			// Ctrl+C

# define  ERR_MEM    	94
# define  ERR_TOKEN   	95
# define  ERR_QUOTE   	96
# define  ERR_SYNTAX  	97
# define  ERR_INIT_STAT 98
# define  ERR_EXECVE	99
# define  ERR_UNDEFINED 100

#endif
