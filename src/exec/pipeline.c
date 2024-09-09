/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:00:25 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/09 21:00:27 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	initialize_pipeline(t_ast *root, t_data *msh)
{
	pid_t	ps;
	int		status;

	if (!root || !msh)
		return (1);
	status = 0;
	ps = fork();
	if (!ps)
	{
		msh->child_ps = NULL;
		setup_pipeline(root, msh);
		output_to_stdout();
		ft_waitpid(msh);
		exit(get_exit_code());
	}
	waitpid(ps, &status, 0);
	set_exit_code(msh->env, WEXITSTATUS(status));
	return (0);
}

void	setup_pipeline(t_ast *root, t_data *msh)
{
	if (!root || !msh)
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
	msh->child_ps = NULL;
	if (dup2(pdes[1], STDOUT_FILENO) == -1)
		panic(msh);
	close(pdes[0]);
	close(pdes[1]);
	if (cmd->type != COMMAND)
	{
		travers_tree((t_ast *)cmd, msh);
		ft_waitpid(msh);
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
