/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/builtins/unset.c
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/13 19:02:35 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_unset(char **args, t_list *env)
{
	t_env	*node;

	if (!args || !env)
		return (EXIT_FAILURE);
	if (!args[1])
		return (EXIT_SUCCESS);
	node = get_env(env, args[1]);
	if (node)
		env_delete(env, args[1]);
	return (EXIT_SUCCESS);
}
=======
/*   Created: 2024/03/12 14:40:26 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/03/12 14:42:28 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

char	ft_printu(size_t num, size_t base, char cap);
char	ft_printx(size_t num, char ch);
size_t	multiplexer(char ch, va_list *args);
int		ft_printf(const char *s, ...);
char	ft_putchar(int c);
size_t	ft_putstr(char *s);
char	ft_putnbr(int n);

#endif
>>>>>>> origin/parser:src/ft_printf/ft_printf.h
