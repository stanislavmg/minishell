/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:48:21 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/24 08:51:50 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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

char	**ft_first_split(char const *s, char c)
{
	char	**res;
	int		count;

	count = 0;
	res = malloc(3 * sizeof(char *));
	if (!res)
		return (NULL);
	while (s[count] != c)
		count++;
	res[0] = malloc(count + 1);
	if (!res[0])
		return (free_res(res, 0));
	ft_strlcpy(res[0], s, count + 1);
	count = ft_strlen(s) - count;
	res[1] = malloc(count + 1);
	if (!res[1])
		return (free_res(res, 1));
	ft_strlcpy(res[1], s + (ft_strlen(s) - count + 1), count + 1);
	res[2] = NULL;
	return (res);
}
