/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:01:26 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/31 00:19:17 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "parser.h"

int	ft_fnmatch(char *pattern, char *filename)
{
	char	*star_pos;
	char	*match;

	star_pos = NULL;
	match = NULL;
	while (*filename)
	{
		if (*pattern == *filename)
		{
			pattern++;
			filename++;
		}
		else if (*pattern == '*')
		{
			star_pos = pattern++;
			match = filename;
		}
		else
		{
			if (!star_pos)
				return (0);
			pattern = star_pos + 1;
			filename = ++match;
		}
	}
	while (*pattern == '*')
		pattern++;
	return (!*pattern && !*filename);
}

void	expand_wildcard(t_lexer *lex, char *pattern)
{
	DIR				*cwd;
	struct dirent	*dr;

	cwd = opendir(".");
	dr = readdir(cwd);
	while (dr)
	{
		while (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, ".."))
			dr = readdir(cwd);
		if (ft_fnmatch(pattern, dr->d_name))
			push_token(&lex->tokens, ft_strdup(dr->d_name), STRING);
		dr = readdir(cwd);
	}
	closedir(cwd);
}
