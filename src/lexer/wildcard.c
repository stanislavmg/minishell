/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 00:01:26 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 17:44:13 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "parser.h"

int	ft_fnmatch(char *pattern, char *filename, char *star_pos, char *match)
{
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
		while (!ft_strcmp(dr->d_name, ".") || !ft_strcmp(dr->d_name, ".."))
			dr = readdir(cwd);
		if (ft_fnmatch(pattern, dr->d_name, NULL, NULL))
			push_token(&lex->tokens, ft_strdup(dr->d_name), ARG);
		dr = readdir(cwd);
	}
	closedir(cwd);
}
