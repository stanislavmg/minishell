/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:53 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/17 13:16:29 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "exec.h"
# include "builtins.h"

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
		if (get_last_status(msh->env))
			travers_tree((t_ast *)root->right, msh);
	}
	else if (root->type == AND)
	{
		travers_tree((t_ast *)root->left, msh);
		if (get_last_status(msh->env) == EXIT_SUCCESS) 
			travers_tree((t_ast *)root->right, msh);
	}
	else if (root->type == PIPE)
		start_pipeline(root, msh);
	else if (root->type == REDIRECT)
		open_redirect(root, msh);
	else if (root->type == SEMICOLON)
	{
		travers_tree((t_ast *)root->left, msh);
		travers_tree((t_ast *)root->right, msh);
	}
	return (0);
}

int is_builtin(const char *cmd)
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


void	start_job(t_exec_cmd *cmd,  t_data *msh)
{
	pid_t	pid;
	int 	exit_code;
	char	**envp;

	envp = NULL;
	exit_code = 0;
	if (is_builtin(cmd->argv[0]))
	{
		exit_code = handle_command(cmd->argv, msh);
		set_last_status(msh->env, exit_code);
	}
	else
	{
		envp = new_env_arr(msh->env);
		pid = fork();
		if (!pid)
			ft_execve(cmd, envp);
		if (pid == -1)
			panic(msh);
		waitpid(pid, &exit_code, 0);
		free_arr(envp);
		set_last_status(msh->env, WEXITSTATUS(exit_code));
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
	while (env[i] && strncmp(env[i], "PATH=", 5))
		i++;
	paths = get_path(env[i] + 5);
	cmd->path = parsing_path(paths, cmd->argv[0]);
	free_arr(paths);
	if (access(cmd->argv[0], F_OK) || !cmd->path)
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

void start_first_ps(t_ast *root, t_data *msh, int *pdes)
{
	if (root->type != SEMICOLON)
	{
		dup2(pdes[1], STDOUT_FILENO);
		close(pdes[0]);
		close(pdes[1]);
		travers_tree(root, msh);
	}
	else
	{
		travers_tree((t_ast *)root->left, msh); // FIXME
		dup2(pdes[1], STDOUT_FILENO);
		close(pdes[0]);
		close(pdes[1]);
		travers_tree((t_ast *)root->right, msh);
	}
	exit(get_last_status(msh->env));
}

void start_second_ps(t_ast *root, t_data *msh, int *pdes)
{
	if (root->type != SEMICOLON)
	{
		dup2(pdes[0], STDIN_FILENO);
		close(pdes[1]);
		close(pdes[0]);
		travers_tree(root, msh);
	}
	else
	{
		travers_tree((t_ast *)root->left, msh);
		dup2(pdes[0], STDIN_FILENO);
		ft_close(pdes[1]);
		ft_close(pdes[0]);
		travers_tree((t_ast *)root->right, msh);
	}
	exit(get_last_status(msh->env));
}


int	start_pipeline(t_ast *root, t_data *msh)
{
	int		exit_code;
	pid_t	ps1;
	pid_t	ps2;
	int		pdes[2];

	exit_code = 0;
	if (!root)
		return (0);
	if (pipe(pdes) == -1)
		panic(msh);
    ps1 = fork();
    if (ps1 == 0)
		start_first_ps((t_ast *)root->left, msh, pdes);
	else if (ps1 < 0)
		panic(msh);
	else if (ps1 > 0)
	{
		ps2 = fork();
		if (ps2 == 0)
			start_second_ps((t_ast *)root->right, msh, pdes);
		else if (ps2 < 0)
			panic(msh);
		ft_close(pdes[0]);
        ft_close(pdes[1]);
		waitpid(ps1, &exit_code, 0);
		set_last_status(msh->env, WEXITSTATUS(exit_code));
		waitpid(ps2, &exit_code, 0);
		set_last_status(msh->env, WEXITSTATUS(exit_code));
    }
	else
		panic(msh);
	return (exit_code);
}

int	open_redirect(t_ast *root, t_data *msh)
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
		travers_tree((t_ast *)root->left, msh);
		exit(get_last_status(msh->env));
	}
	else if (pid == -1)
		panic(msh);
	else
	{
		waitpid(pid, &exit_code, 0);
		set_last_status(msh->env, WEXITSTATUS(exit_code));
	}
	return (exit_code);
}
