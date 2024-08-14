/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:53 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/14 18:06:18 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "exec.h"
# include "builtins.h"

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
	{
		handle_command(cmd->argv, list_env);
		ft_execve(cmd, envp);
	}
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", STDERR_FILENO);
		return ;
	}
	waitpid(pid, &exit_code, 0);
	free_arr(envp);
	set_last_status(list_env, WEXITSTATUS(exit_code));
}

int	ft_execve(t_exec_cmd *cmd, char **env)
{
	char	**paths;
	int		i;

	i = 0;
	paths = NULL;
	
	while (env[i] && strncmp(env[i], "PATH=", 5))
		i++;
	paths = get_path(env[i] + 5);
	cmd->path = parsing_path(paths, cmd->argv[0]);
	free_arr(paths);
	if (access(cmd->path, F_OK) || !cmd->path)
		exit_failure(cmd->argv[0], CMD_NOT_FOUND);
	if (access(cmd->path, X_OK))
		exit_failure(cmd->argv[0], PERM_DENIED);
	if (cmd->path)
		execve(cmd->path, cmd->argv, env);
	perror(cmd->argv[0]);
	exit(EXIT_FAILURE);
}
int is_logic_operator(e_token type)
{
	return (type == OR || type == AND || type == SEMICOLON);
}

void start_first_ps(t_ast *root, t_list *env, int *pdes)
{
	if (root->type != SEMICOLON)
	{
		dup2(pdes[1], STDOUT_FILENO);
		close(pdes[0]);
		close(pdes[1]);
		travers_tree(root, env);
	}
	else
	{
		travers_tree((t_ast *)root->left, env); // FIXME
		dup2(pdes[1], STDOUT_FILENO);
		close(pdes[0]);
		close(pdes[1]);
		travers_tree((t_ast *)root->right, env);
	}
	exit(get_last_status(env));
}

void start_second_ps(t_ast *root, t_list *env, int *pdes)
{
	if (root->type != SEMICOLON)
	{
		dup2(pdes[0], STDIN_FILENO);
		close(pdes[1]);
		close(pdes[0]);
		travers_tree(root, env);
	}
	else
	{
		travers_tree((t_ast *)root->left, env);
		dup2(pdes[0], STDIN_FILENO);
		ft_close(pdes[1]);
		ft_close(pdes[0]);
		travers_tree((t_ast *)root->right, env); // FIXME
	}
	exit(get_last_status(env));
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
		exit_failure("pipe", EXIT_FAILURE);
	exit_code = 0;
    ps1 = fork();
    if (ps1 == 0)
		start_first_ps((t_ast *)root->left, list_env, pdes);
	else if (ps1 < 0)
		exit_failure("fork", EXIT_FAILURE); // FIXME need free
	else if (ps1 > 0)
	{
		ps2 = fork();
		if (ps2 == 0)
			start_second_ps((t_ast *)root->right, list_env, pdes);
		else if (ps2 < 0)
			exit_failure("fork", EXIT_FAILURE);
		close(pdes[0]);
        close(pdes[1]);
		waitpid(ps1, &exit_code, 0);
		set_last_status(list_env, WEXITSTATUS(exit_code));
		waitpid(ps2, &exit_code, 0);
		set_last_status(list_env, WEXITSTATUS(exit_code));
    }
	else
		exit_failure("fork", EXIT_FAILURE); // FIXME need free
	return (exit_code);
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
    pid = fork();
	if (!pid)
	{
		fd = ft_open(rfile->fname, rfile->mode);
		if (fd == -1)
			exit_failure(rfile->fname, PERM_DENIED);
		if (rfile->type == INPUT_TRUNC || rfile->type == HERE_DOC)
        	dup2(fd, STDIN_FILENO);
		else
        	dup2(fd, STDOUT_FILENO);
		ft_close(fd);
		exit(travers_tree((t_ast *)root->left, list_env));
	}
	else
	{
		wait(&exit_code);
		set_last_status(list_env, WEXITSTATUS(exit_code));
	}
	return (exit_code);
}
