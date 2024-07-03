/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:54 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/04 15:52:51 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*d;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	if (start < ft_strlen(s))
		i = ft_strlen(s) - start;
	if (i > len)
		i = len;
	d = malloc(i + 1);
	if (!d)
		return (NULL);
	ft_strlcpy(d, s + start, i + 1);
	return (d);
}
