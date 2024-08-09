/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:53 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/09 18:22:30 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "exec.h"

int	travers_tree(t_ast *root, t_list *list_env)
{
	if (!root)
		return (0);
	if (root->type == COMMAND)
		start_job(list_env, (t_exec_cmd *)root);
	else if (root->type == VARIABLE)
		set_env(list_env, (t_var *)root, HIDDEN);
	else if (root->type == OR)
	{
		travers_tree((t_ast *)root->left, list_env);
		if (get_last_status(list_env))
			travers_tree((t_ast *)root->right, list_env);
	}
	else if (root->type == AND)
	{
		travers_tree((t_ast *)root->left, list_env);
		if (get_last_status(list_env) == EXIT_SUCCESS) 
			travers_tree((t_ast *)root->right, list_env);
	}
	else if (root->type == PIPE)
		start_pipeline(root, list_env);
	else if (root->type == REDIRECT)
		open_redirect(root, list_env);
	else if (root->type == SEMICOLON)
	{
		travers_tree((t_ast *)root->left, list_env);
		travers_tree((t_ast *)root->right, list_env);
	}
	return (0);
}

void	start_job(t_list *list_env, t_exec_cmd *cmd)
{
	pid_t	pid;
	int 	exit_code;
	char	**envp;

	envp = new_env_arr(list_env);
	pid = fork();
	if (!pid)
		ft_execve(cmd, envp);
	if (pid == -1)
	{
		printf("fork error\n");
		return ;
	}
	waitpid(pid, &exit_code, 0);
	free_arr(envp);
	set_last_status(list_env, exit_code);
}

int	ft_execve(t_exec_cmd *cmd, char **env)
{
	char	**paths;

	paths = NULL;
	while (*env && strncmp(*env, "PATH=", 5))
		env++;
	paths = get_path(*env + 5);
	cmd->path = parsing_path(paths, cmd->argv[0]);
	free_arr(paths);
	if (cmd->path)
		execve(cmd->path, cmd->argv, env);
	else
	{
		printf("minishell: %s: command not found\n", cmd->argv[0]);
		exit(EXIT_FAILURE);
	}
	perror(cmd->argv[0]);
	exit(EXIT_FAILURE);
}

int	start_pipeline(t_ast *root, t_list *list_env)
{
	int		exit_code;
	pid_t	ps1;
	pid_t	ps2;
	int		pdes[2];

	if (!root)
		return (0);
	if (pipe(pdes) == -1)
	{
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    ps1 = fork();
    if (ps1 == 0)
	{
        dup2(pdes[1], STDOUT_FILENO);
        close(pdes[0]);
        close(pdes[1]);
        exit(travers_tree((t_ast *)root->left, list_env));
    }
	else if (ps1 > 0)
	{
		ps2 = fork();
		if (ps2 == 0) {
        	ft_close(pdes[1]);
        	dup2(pdes[0], STDIN_FILENO);
        	ft_close(pdes[0]);
			waitpid(ps1, &exit_code, 0);
			set_last_status(list_env, exit_code);
        	exit(travers_tree((t_ast *)root->right, list_env));
		}
		close(pdes[0]);
        close(pdes[1]);
		waitpid(ps2, &exit_code, 0);
		set_last_status(list_env, exit_code);
    }
	else
	{
        perror("fork");
        return -1;
    }
	return (exit_code);
}

int open_io(t_redir *io_file)
{
	int	fd;

	fd = ft_open(io_file->fname, io_file->mode);
	if (fd == -1)
		return (1);

	ft_close(fd);
	return (0);
}

int	open_redirect(t_ast *root, t_list *list_env)
{
	int		exit_code;
	t_redir	*rfile;
	pid_t	pid;
	int		fd;

	if (!root)
		return (1);
	exit_code = 0;
	rfile = (t_redir *)root->right;
	fd = ft_open(rfile->fname, rfile->mode);
	if (fd == -1)
		exit_failure(rfile->fname);
    pid = fork();
	if (!pid)
	{
		if (rfile->type == INPUT_TRUNC || rfile->type == HERE_DOC)
        	dup2(fd, STDIN_FILENO);
		else
        	dup2(fd, STDOUT_FILENO);
		exit(travers_tree((t_ast *)root->left, list_env));
	}
	else
	{
		waitpid(pid, &exit_code, 0);
		set_last_status(list_env, exit_code);
	}
	return (exit_code);
}
