#include "minishell.h"
#include "parser.h"

int	ft_fnmatch(char *pattern, char *filename)
{
	char *star_pos;
	char *match;

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

t_list *expand_wildcard(char *pattern)
{
	DIR *cwd;
	struct dirent *dr;
	t_list *args;

	cwd = opendir(".");
	args = NULL;
	while ((dr = readdir(cwd)))
	{
		while (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, ".."))
			dr = readdir(cwd);
		if (ft_fnmatch(pattern, dr->d_name))
			ft_lstadd_back(&args, ft_lstnew(ft_strdup(dr->d_name)));
	}
	closedir(cwd);
	return (args);
}
