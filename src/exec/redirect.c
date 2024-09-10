/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:28:15 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 20:48:18 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	swap_sdtstream(char *file_name, int mode);
static int	reset_stdstream(int saved_stdin, int saved_stdout);

int	open_redirect(t_ast *root, t_redir *rfile, t_data *msh)
{
	int	fd;
	int	saved_stdout;
	int	saved_stdin;

	if (!rfile || !msh)
		return (1);
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (!check_file_permission(rfile->fname, rfile->mode))
		fd = swap_sdtstream(rfile->fname, rfile->mode);
	else
	{
		set_exit_code(msh->env, EXIT_FAILURE);
		close(saved_stdin);
		close(saved_stdout);
		return (1);
	}
	set_exit_code(msh->env, EXIT_SUCCESS);
	if (root)
		travers_tree((t_ast *)root->left, msh);
	close(fd);
	if (reset_stdstream(saved_stdin, saved_stdout))
		panic(msh);
	return (0);
}

static int	swap_sdtstream(char *file_name, int mode)
{
	int	fd;

	fd = open(file_name, mode, 0644);
	if (mode == O_RDONLY)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			return (-1);
		}
	}
	return (fd);
}

static int	reset_stdstream(int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (1);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (1);
	close(saved_stdin);
	close(saved_stdout);
	return (0);
}
