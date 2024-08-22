/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:53 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/21 12:17:09 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "builtins.h"

int	g_exit_code;

int	travers_tree(t_ast *root, t_data *msh)
{
	if (!root)
		return (0);
	if (root->type == COMMAND)
		start_job((t_exec_cmd *)root, msh);
	else if (root->type == VARIABLE)
		set_env(msh->env, (t_var *)root, HIDDEN);
	else if (root->type == OR)
	{
		travers_tree((t_ast *)root->left, msh);
		ft_waitpid(msh);
		if (get_exit_code())
			travers_tree((t_ast *)root->right, msh);
	}
	else if (root->type == AND)
	{
		travers_tree((t_ast *)root->left, msh);
		ft_waitpid(msh);
		if (!get_exit_code())
			travers_tree((t_ast *)root->right, msh);
	}
	else if (root->type == PIPE)
		start_pipeline(root, msh);
	else if (root->type == REDIRECT)
		open_redirect(root, msh);
	else if (root->type == SEMICOLON)
	{
		travers_tree((t_ast *)root->left, msh);
		ft_waitpid(msh);
		travers_tree((t_ast *)root->right, msh);
	}
	return (0);
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (strcmp(cmd, "cd") == 0)
		return (1);
	else if (strcmp(cmd, "env") == 0)
		return (1);
	else if (strcmp(cmd, "pwd") == 0)
		return (1);
	else if (strcmp(cmd, "export") == 0)
		return (1);
	else if (strcmp(cmd, "echo") == 0)
		return (1);
	else if (strcmp(cmd, "unset") == 0)
		return (1);
	else if (strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

void	start_job(t_exec_cmd *cmd, t_data *msh)
{
	pid_t	*pid;
	char	**envp;

	envp = NULL;
	pid = (pid_t *)malloc(sizeof(pid_t));
	if (!pid)
		panic(msh);
	if (is_builtin(cmd->argv[0]))
		set_last_status(msh->env, handle_command(cmd->argv, msh));
	else
	{
		envp = new_env_arr(msh->env);
		*pid = fork();
		if (!*pid)
			ft_execve(cmd, envp);
		if (*pid == -1)
			panic(msh);
		free_arr(envp);
		ft_lstadd_back(&msh->child_ps, ft_lstnew(pid));
	}
}

void	close_fds_except_std(void)
{
	int	fd;
	int	max = sysconf(_SC_OPEN_MAX); //FIXME need user const

	fd = 2;
	while (++fd < max)
		close(fd);
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
	else if(S_ISDIR(file_info.st_mode))
		exit_failure(cmd->path, CMD_NOT_FOUND);
}

int	ft_execve(t_exec_cmd *cmd, char **env)
{
	char	**paths;
	int		i;

	i = 0;
	paths = NULL;
	if (!cmd || !cmd->argv)
		exit(EXIT_FAILURE);
	close_fds_except_std();
	while (env[i] && strncmp(env[i], "PATH=", 5))
		i++;
	paths = get_path(env[i] + 5);
	cmd->path = parsing_path(paths, cmd->argv[0]);
	free_arr(paths);
	check_cmd_permission(cmd);
	execve(cmd->path, cmd->argv, env);
	exit_failure(cmd->argv[0], ERR_EXECVE);
	return (0);
}

int	is_logic_operator(e_token type)
{
	return (type == OR || type == AND || type == SEMICOLON);
}

void	ft_waitpid(t_data *msh)
{
	t_list	*t;
	int		status;

	if (!msh || !msh->child_ps)
		return ;
	t = msh->child_ps->next;
	waitpid((*(pid_t *)msh->child_ps->data), &status, 0);
	set_last_status(msh->env, WEXITSTATUS(status));
	ft_lstdelone(msh->child_ps, free);
	msh->child_ps = t;
}

int	exec_first_ps(t_data *msh, t_ast *root, int out_pdes)
{
	if (root->type != SEMICOLON)
	{
		if (dup2(out_pdes, STDOUT_FILENO) == -1)
			return (1);
		close(out_pdes);
		travers_tree(root, msh);
	}
	else
	{
		travers_tree((t_ast *)root->left, msh);
		ft_waitpid(msh);
		if (dup2(out_pdes, STDOUT_FILENO) == -1)
			return (1);
		close(out_pdes);
		travers_tree((t_ast *)root->right, msh);
	}
	return (0);
}

int	exec_second_ps(t_data *msh, t_ast *root, int in_pdes)
{
	if (root->type != SEMICOLON)
	{
		if (dup2(in_pdes, STDIN_FILENO) == -1)
			return (1);
		close(in_pdes);
		travers_tree(root, msh);
	}
	else
	{
		travers_tree((t_ast *)root->left, msh);
		ft_waitpid(msh);
		if (dup2(in_pdes, STDIN_FILENO) == -1)
			return (1);
		close(in_pdes);
		travers_tree((t_ast *)root->right, msh);
	}
	return (0);
}

int	start_pipeline(t_ast *root, t_data *msh)
{
	int	pdes[2];
	int	saved_stdout;
	int	saved_stdin;

	if (!root)
		return (0);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdout == -1 || saved_stdin == -1)
		return (1);
	if (pipe(pdes) == -1)
		panic(msh);
	if (exec_first_ps(msh, (t_ast *)root->left, pdes[1]))
		panic(msh);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (1);
	close(saved_stdout);
	if (exec_second_ps(msh, (t_ast *)root->right, pdes[0]))
		panic(msh);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		panic(msh);
	close(saved_stdout);
	close(saved_stdin);
	close(pdes[0]);
	close(pdes[1]);
	return (0);
}

int	open_redirect(t_ast *root, t_data *msh)
{
	t_redir	*rfile;
	int		saved_stdout;
	int		saved_stdin;
	int		fd;

	if (!root)
		return (1);
	fd = -1;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	rfile = (t_redir *)root->right;
	if (!check_file_permission(rfile->fname, rfile->mode))
		fd = open(rfile->fname, rfile->mode, 0644);
	else
	{
		set_last_status(msh->env, 1);
		return (1);
	}
	if (rfile->type == INPUT_TRUNC || rfile->type == HERE_DOC)
	{
		if (fd > 0)
			dup2(fd, STDIN_FILENO);
		travers_tree((t_ast *)root->left, msh);
		dup2(saved_stdin, STDIN_FILENO);
	}
	else
	{
		if (fd > 0)
			dup2(fd, STDOUT_FILENO);
		travers_tree((t_ast *)root->left, msh);
		dup2(saved_stdout, STDOUT_FILENO);
	}
	ft_close(fd);
	ft_close(saved_stdin);
	ft_close(saved_stdout);
	return (0);
}
