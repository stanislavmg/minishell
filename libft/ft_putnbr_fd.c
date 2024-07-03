/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:55:12 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 16:55:13 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	num;

	if (n < 0)
		num = n * -1;
	else
		num = n;
	if (n / 10 == 0)
	{
		if (n < 0)
			ft_putchar_fd('-', fd);
		ft_putchar_fd('0' + num % 10, fd);
		return ;
	}
	ft_putnbr_fd(n / 10, fd);
	ft_putchar_fd('0' + num % 10, fd);
}
