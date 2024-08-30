/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:55:37 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 15:26:52 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	initialize_pipeline(t_ast *root, t_data *msh)
{
	pid_t	ps;
	int		exit_code;

	exit_code = 0;
	ps = fork();
	if (!ps)
	{
		setup_pipeline(root, msh);
		while (msh->child_ps)
			ft_waitpid(msh);
		if (fd_is_pipe(STDIN_FILENO))
			output_to_stdout();
		exit(get_exit_code());
	}
	waitpid(ps, &exit_code, 0);
	set_exit_code(msh->env, WEXITSTATUS(exit_code));
	return (0);
}

void	setup_pipeline(t_ast *root, t_data *msh)
{
	if (root == NULL)
		return ;
	if (root->type == PIPE)
	{
		setup_pipeline((t_ast *)root->left, msh);
		setup_pipeline((t_ast *)root->right, msh);
	}
	else if (root->type == SEMICOLON)
	{
		travers_tree((t_ast *)root->left, msh);
		create_pipeline(msh, (t_exec_cmd *)root->right);
	}
	else
		create_pipeline(msh, (t_exec_cmd *)root);
}

void	run_child_ps(int *pdes, t_exec_cmd *cmd, t_data *msh)
{
	if (dup2(pdes[1], STDOUT_FILENO) == -1)
		panic(msh);
	close(pdes[0]);
	close(pdes[1]);
	if (cmd->type != COMMAND)
	{
		travers_tree((t_ast *)cmd, msh);
		exit(get_exit_code());
	}
	if (is_builtin(cmd->argv[0]))
		exit(handle_command(cmd->argv, msh));
	ft_execve(cmd, new_env_arr(msh->env));
}

void	create_pipeline(t_data *msh, t_exec_cmd *first)
{
	int		pdes[2];
	pid_t	ps;

	if (pipe(pdes) == -1)
		panic(msh);
	ps = fork();
	if (ps == -1)
		panic(msh);
	else if (ps == 0)
		run_child_ps(pdes, first, msh);
	else
		record_pid(msh, ps);
	if (dup2(pdes[0], STDIN_FILENO) == -1)
		panic(msh);
	close(pdes[0]);
	close(pdes[1]);
}

void	output_to_stdout(void)
{
	size_t	ch;
	char	buf[1024];

	ch = read(STDIN_FILENO, buf, 1024);
	while (ch > 0)
	{
		if (write(STDOUT_FILENO, buf, ch) == -1)
			return ;
		ch = read(STDIN_FILENO, buf, 1024);
	}
}
