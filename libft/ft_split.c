/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:55:34 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/16 14:47:39 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	const	*skip_ch(char const *s, char c)
{
	while (*s == c)
		s++;
	return (s);
}

static size_t	count_str(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static char	**get_str(size_t *count, char const *s, char c)
{
	char	**res;

	if (!s)
		return (NULL);
	*count = count_str(s, c);
	res = malloc((*count + 1) * sizeof(char *));
	return (res);
}

static char	**free_res(char **res, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
		free(res[i++]);
	free(res);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	count;
	size_t	i;
	size_t	j;
	char	**res;

	i = 0;
	res = get_str(&count, s, c);
	if (!res)
		return (NULL);
	while (i < count)
	{
		j = 0;
		s = skip_ch(s, c);
		while (s[j] != c && s[j])
			j++;
		res[i] = malloc(j + 1);
		if (!res[i])
			return (free_res(res, i));
		ft_strlcpy(res[i], s, j + 1);
		s = skip_ch(s + j, c);
		i++;
	}
	res[i] = NULL;
	return (res);
}
