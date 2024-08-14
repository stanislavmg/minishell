/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/builtins/echo.c
/*   Created: 2024/07/23 07:16:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/13 19:08:09 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handele_echo(char **args)
=======
/*   Created: 2024/03/12 14:40:17 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/03/12 14:41:44 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *s, ...)
>>>>>>> origin/parser:src/ft_printf/ft_printf.c
{
	int		i;
	size_t	count;
	va_list	args;

<<<<<<< HEAD:src/builtins/echo.c
	i = 1;
	flag = 0;
	if (!args)
	while (ft_strcmp(args[i], "-n") == 0)
=======
	i = 0;
	count = 0;
	va_start(args, s);
	if (!s)
		return (0);
	while (s[i])
>>>>>>> origin/parser:src/ft_printf/ft_printf.c
	{
		if (s[i] == '%')
		{
			count += i;
			write(1, s, i++);
			count += multiplexer(s[i], &args);
			s += i + 1;
			i = -1;
		}
		i++;
	}
<<<<<<< HEAD:src/builtins/echo.c
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (!flag)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
=======
	count += ft_putstr((char *)s);
	va_end(args);
	return (count);
>>>>>>> origin/parser:src/ft_printf/ft_printf.c
}
