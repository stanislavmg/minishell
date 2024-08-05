/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:48:35 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 16:22:45 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h" // просто "builtins.h"

int	handle_command(char **args, t_env *env, int *fd)
{
	int	result;

// if !args || !env
// fd не нужен
	if (strcmp(args[0], "cd") == 0)
		result = handle_cd(args, env, fd);
	else if (strcmp(args[0], "env") == 0)
		result = handle_env(args, env, fd);
	else if (strcmp(args[0], "pwd") == 0)
		result = handle_pwd(args, env, fd);
	else if (strcmp(args[0], "export") == 0)	
		result = handle_export(args, env, fd);
	else if (strcmp(args[0], "echo") == 0)	
		result = handele_echo(args, env, fd);
	else if (strcmp(args[0], "unset") == 0)	
		result = handle_unset(args, env, fd);
	else if (strcmp(args[0], "exit") == 0)	
		result = handle_exit(args, env, fd);
	else
		result = execute_command(args, env, fd);
	return (result);
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	//if !arr
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL; //  не надо, т.к память уже не наша
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	// if !s1 !s2
	while (s1[i] == s2[i] && s1[i] && s2[i]) //SEGFAULT
		i++;
	return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
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
	size_t	i;
	size_t	j;
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
