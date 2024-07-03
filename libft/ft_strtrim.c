/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:58:44 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 19:33:05 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	isset(char ch, char const *set)
{
	unsigned int	i;

	i = 0;
	while (set[i])
	{
		if (ch == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		start;
	int		end;
	int		num;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	end = ft_strlen(s1) - 1;
	while (isset(s1[start], set) && s1[start])
		start++;
	while (isset(s1[end], set) && end > 0)
		end--;
	if (start > end)
		num = 0;
	else
		num = end - start + 1;
	res = malloc(num + 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1 + start, num + 1);
	return (res);
}
