/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:49:24 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 16:49:26 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	putnbr(int pos, int n, char *str)
{
	unsigned int	num;

	if (n < 0)
		num = n * -1;
	else
		num = n;
	if (n / 10 == 0)
	{
		str[pos] = '0' + num % 10;
		if (n < 0)
			str[pos - 1] = '-';
		return ;
	}
	putnbr(pos - 1, n / 10, str);
	str[pos] = '0' + num % 10;
}

char	*ft_itoa(int n)
{
	size_t	len;
	int		num;
	char	*res;

	len = 1;
	num = n;
	if (n < 0)
		len++;
	else if (n == 0)
		len++;
	while (num)
	{
		num /= 10;
		len++;
	}
	res = malloc(len);
	if (!res)
		return (NULL);
	res[len - 1] = 0;
	putnbr(len - 2, n, res);
	return (res);
}
