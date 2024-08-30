/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:40:36 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 17:41:09 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	**get_path(char *path_env)
{
	char	**path;
	char	*tmp;
	int		i;

	i = -1;
	path = ft_split(path_env, ':');
	if (!path)
		return (NULL);
	while (path[++i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path[i], "/");
			if (!tmp)
			{
				free_arr(path);
				return (NULL);
			}
			free(path[i]);
			path[i] = tmp;
		}
	}
	return (path);
}

char	*parsing_path(char **path_env, char *cmd_name)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!path_env || !*path_env || !cmd_name)
		return (NULL);
	cmd_path = (char *)malloc(sizeof(char) * ft_strlen(cmd_name) + 1);
	if (!cmd_path)
		return (NULL);
	ft_strlcpy(cmd_path, cmd_name, ft_strlen(cmd_name) + 1);
	while (path_env[i])
	{
		if (!access(cmd_path, F_OK | X_OK))
			return (cmd_path);
		free(cmd_path);
		cmd_path = ft_strjoin(path_env[i], cmd_name);
		if (!cmd_path)
			return (NULL);
		i++;
	}
	free(cmd_path);
	return (NULL);
}

t_exec_cmd	*new_exec_cmd(t_list *args)
{
	t_exec_cmd	*new_cmd;

	new_cmd = (t_exec_cmd *)ft_calloc(1, sizeof(t_exec_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->type = COMMAND;
	new_cmd->argv = add_field_argv(args);
	return (new_cmd);
}

char	**add_field_argv(t_list *args)
{
	char	**argv;
	int		argc;
	int		i;

	if (!args)
		return (NULL);
	i = -1;
	argc = ft_lstsize(args) + 1;
	argv = (char **)malloc(sizeof(char *) * argc);
	if (!argv)
		return (NULL);
	while (++i < argc - 1)
	{
		argv[i] = (char *)args->data;
		args->data = NULL;
		args = args->next;
	}
	argv[i] = NULL;
	return (argv);
}
