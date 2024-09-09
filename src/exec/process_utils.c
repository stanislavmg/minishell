/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:01:00 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 21:01:04 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	record_pid(t_data *msh, pid_t pid)
{
	pid_t	*new_pid;

	new_pid = (pid_t *)malloc(sizeof(pid_t));
	if (!pid)
		panic(msh);
	*new_pid = pid;
	ft_lstadd_back(&msh->child_ps, ft_lstnew(new_pid));
}

void	ft_waitpid(t_data *msh)
{
	t_list	*t;
	int		status;

	if (!msh || !msh->child_ps)
		return ;
	status = 0;
	while (msh->child_ps)
	{
		t = msh->child_ps->next;
		waitpid((*(pid_t *)msh->child_ps->data), &status, 0);
		if (WIFSIGNALED(status))
			set_exit_code(msh->env, FT_SIGINT);
		else
			set_exit_code(msh->env, WEXITSTATUS(status));
		ft_lstdelone(msh->child_ps, free);
		msh->child_ps = t;
	}
}

int	ft_execve(t_exec_cmd *cmd, char **env)
{
	char	**paths;
	int		i;

	i = 0;
	paths = NULL;
	if (!cmd || !cmd->argv)
		exit(EXIT_FAILURE);
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (env[i])
		paths = get_path(env[i] + 5);
	else
		paths = NULL;
	cmd->path = parsing_path(paths, cmd->argv[0]);
	free_array(paths);
	check_cmd_permission(cmd);
	execve(cmd->path, cmd->argv, env);
	exit_failure(cmd->argv[0], ERR_EXECVE);
	return (1);
}

void	check_cmd_permission(t_exec_cmd *cmd)
{
	struct stat	file_info;

	if (!cmd->path)
		exit_failure(cmd->argv[0], CMD_NOT_FOUND);
	if (stat(cmd->path, &file_info))
		exit_failure(cmd->path, ERR_INIT_STAT);
	if (!(file_info.st_mode & S_IXUSR)
		&& (file_info.st_mode & (S_IXGRP | S_IXOTH)))
		exit_failure(cmd->argv[0], PERM_DENIED);
	else if (!(file_info.st_mode & S_IXUSR))
		exit_failure(cmd->argv[0], PERM_DENIED);
	if (S_ISDIR(file_info.st_mode) && ft_strchr(cmd->path, '/'))
		exit_failure(cmd->path, ERR_IS_DIR);
	else if (S_ISDIR(file_info.st_mode))
		exit_failure(cmd->path, CMD_NOT_FOUND);
}
