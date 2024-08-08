/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:02 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/07 10:29:18 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define CMD_NOT_FOUND	127 		// zsh: command not found: cmd_name
# define INVALID_ARG_FOR_EXIT 128	// Invalid argument to exit
# define FT_SIGINT	130 			// Ctrl+C

# define  ERR_MEM    	101
# define  ERR_TOKEN   	102
# define  ERR_QUOTE   	103
# define  ERR_SYNTAX  	104

#endif