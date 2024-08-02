# include "exec.h"

void	add_var(t_var *var, t_exec_data *exec_info)
{
	set_env(exec_info->list_env, var->key, var->value);
	var->key = NULL;
	var->value = NULL;
}

// void	scan_node(t_ast *cmd)
// {
// 	if (cmd->type == PIPE)
// 		pipe_handle();
// 	else if (cmd->type == OR)
// 	else if (cmd->type == AND)
// 	else if (cmd->type == VARIABLE)
// 	else if (cmd->type == COMMAND)
// 	else if (cmd->type == SEMICOLON || cmd->type == ASSIGNMENT)
// }

// int	redirect_last_cmd(t_ast *cmd, int out)
// {
// 	pid_t ps;

// 	if (cmd->type == COMMAND)
// 	{
// 		ps = fork();
// 		if (!ps)
// 			exec_child_ps(0, out, cmd);
// 	}
// 	else if (cmd->type == PIPE)
// 	{
// 		pipe_handle();
// 	}

// 	return (0);
// }

// int	pipe_handle(int in, int out, t_ast *root)
// {
// 	int	pdes[2];

// 	if (!root)
// 		return (0);
// 	pipe(pdes);
// 	redirect_last_cmd(root->left, pdes[1]);
// 	pipe(pdes);
	
// 	ft_close(pdes[0]);
// 	ft_close(pdes[1]);
// 	if (in == 0)
// 	return (0);
// }

// int	exec_child_ps(int in, int out, t_exec_cmd *cmd)
// {
// 	if (in == stdin && cmd->in_fname)
// 		in = ft_open(cmd->in_fname, cmd->in_fmode);
// 	if (out == stdout && cmd->out_fname)
// 		out = ft_open(cmd->out_fname, cmd->out_fmode);
// 	if (dup2(in, stdin) == -1)
// 		exit_failure(cmd->argv[0]);
// 	if (dup2(out, stdout) == -1)
// 		exit_failure(cmd->argv[0]);
// 	if (cmd->path)
// 		execve(cmd->path, cmd->argv, NULL);
// 	ft_close(in);
// 	ft_close(out);
// 	return (1);
// }

int exec_pipe(t_exec_cmd *cmd, t_exec_data *exec_info)
{
	pid_t	ps;


	return (1);
}

int start_pipeline(t_ast *root, t_exec_data *exec_info)
{
	int	pdes[2];
	int	exit_code;
	pid_t ps1;
	pid_t ps2;

	if (root->left->type == COMMAND)
	{
		pipe(pdes);
		((t_exec_cmd *)root->left)->pdes = &pdes;
		((t_exec_cmd *)root->right)->pipe_mode = PIPE_OUT;
		ps1 = fork();
		if (ps1 == 0)
			ft_execve(root->left, 0, pdes[1]);
	}
	if (root->right->type == COMMAND)
	{
		((t_exec_cmd *)root->right)->pdes = &pdes;
		((t_exec_cmd *)root->right)->pipe_mode = PIPE_IN;
		ps2 = fork();
		if (ps2 == 0)
			ft_execve(root->right, pdes[0], 1);
	}
	ft_close(pdes[0]);
	ft_close(pdes[1]);
	wait(&exit_code);
	set_last_status(exec_info->list_env, exit_code);
	wait(&exit_code);
	set_last_status(exec_info->list_env, exit_code);
	return (0);
}

int	travers_tree(t_ast *root, t_exec_data *exec_info)
{
	if (!root)
		return (0);
	if (root->type == COMMAND)
		start_job(exec_info, root);
	else if (root->type == VARIABLE)
		add_var(root, exec_info);
	else if (root->type == PIPE)
		start_pipeline(root, exec_info); 
	else if (root->type == OR)
	{
		travers_tree(root->left, exec_info);
		if (get_last_status(exec_info->list_env))
			travers_tree(root->right, exec_info);
	}
	else if (root->type == AND)
	{
		travers_tree(root->left, exec_info);
		if (get_last_status(exec_info->list_env) == EXIT_SUCCESS) 
			travers_tree(root->right, exec_info);
	}
	else
	{
		travers_tree(root->left, exec_info);
		travers_tree(root->right, exec_info);
	}
	return (0);
}

void	start_job(t_exec_data *exec_info, t_exec_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
		ft_execve(cmd, STDIN_FILENO, STDOUT_FILENO);
	if (pid == -1)
		return ;
	waitpid(pid, &cmd->exit_code, 0);
	set_last_status(exec_info->list_env, cmd->exit_code);
}

void open_cmd_files(t_exec_cmd *cmd)
{

}

int	ft_execve(t_exec_cmd *cmd, int in, int out)
{
	// if (cmd->in_fname)
	// {
	// 	ft_close(in);
	// 	in = ft_open(cmd->in_fname, cmd->in_fmode);
	// }
	// if (cmd->out_fname)
	// {
	// 	ft_close(out);
	// 	out = ft_open(cmd->out_fname, cmd->out_fmode);
	// }
	if (cmd->pipe_mode == PIPE_OUT)
	{
		dup2(out, STDOUT_FILENO);
		ft_close(cmd->pdes[0]);
	}
	else if (cmd->pipe_mode == PIPE_IN)
	{
		dup2(in, STDIN_FILENO);
		ft_close(cmd->pdes[1]);
	}
	else if (cmd->pdes)
	{
		ft_close(cmd->pdes[0]);
		ft_close(cmd->pdes[1]);
	}
	// if (dup2(in, STDIN_FILENO) == -1)
	// 	exit_failure(cmd->argv[0]);
	// if (dup2(out, STDOUT_FILENO) == -1)
	// 	exit_failure(cmd->argv[0]);
	if (cmd->path)
		execve(cmd->path, cmd->argv, NULL);
	ft_close(in);
	ft_close(out);
	exit(EXIT_FAILURE);
}

int	ft_close(int fd)
{
	/* If fd isn't std stream */
	if (fd > 3)
	{
		if (close(fd))
			return (-1);
	}
	return (0);
}

int	ft_open(char *fname, int mode)
{
	int	fd;
	int	access_mode;

	/* If mode have a readonly flag then this is input file */
	if (mode & O_RDONLY)
		access_mode = F_OK | R_OK;
	/* Else output file */
	else
		access_mode = F_OK | W_OK; //FIXME output access fail
	if (access(fname, access_mode))
	{
		perror(fname);
		return (-1);
	}
	fd = open(fname, mode);
	if (fd == -1)
	{
		perror(fname);
		fd = 0;
	}
	return (fd);
}