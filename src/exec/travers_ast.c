/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   travers_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 18:03:07 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 15:27:58 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

extern int	g_exit_code;

void	logic_node_handle(t_ast *root, t_data *msh)
{
	int	exit_code;

	travers_tree((t_ast *)root->left, msh);
	ft_waitpid(msh);
	exit_code = get_exit_code();
	if (root->type == OR && exit_code)
		travers_tree((t_ast *)root->right, msh);
	else if (root->type == AND && !exit_code)
		travers_tree((t_ast *)root->right, msh);
	else if (root->type == SEMICOLON)
		travers_tree((t_ast *)root->right, msh);
}

int	travers_tree(t_ast *root, t_data *msh)
{
	if (!root)
		return (0);
	if (is_logic_operator(root->type))
		logic_node_handle(root, msh);
	if (root->type == COMMAND)
		start_job((t_exec_cmd *)root, msh);
	else if (root->type == VARIABLE)
		set_env(msh->env, (t_var *)root, HIDDEN);
	else if (root->type == PIPE)
		initialize_pipeline(root, msh);
	else if (root->type == REDIRECT)
		open_redirect(root, (t_redir *)root->right, msh);
	return (0);
}

void	start_job(t_exec_cmd *cmd, t_data *msh)
{
	pid_t	pid;
	char	**envp;

	envp = NULL;
	if (is_builtin(cmd->argv[0]))
		set_exit_code(msh->env, handle_command(cmd->argv, msh));
	else
	{
		envp = new_env_arr(msh->env);
		pid = fork();
		if (!pid)
			ft_execve(cmd, envp);
		if (pid == -1)
			panic(msh);
		free_array(envp);
		record_pid(msh, pid);
	}
}
