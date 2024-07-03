/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:56:31 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 16:56:35 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dst_len;
	size_t	src_len;

	if (!size && (!dst || !src))
		return (0);
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size == 0 || size <= dst_len)
		return (size + src_len);
	i = dst_len;
	j = 0;
	while (src[j] && j < (size - dst_len - 1))
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	dst[i] = 0;
	return (src_len + dst_len);
}
