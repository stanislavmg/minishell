/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/17 12:50:25 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "parser.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

int			get_last_status(t_list *env);
void		set_last_status(t_list *list_env, int new_value);
int			start_pipeline(t_ast *root, t_data *msh);
int			ft_execve(t_exec_cmd *cmd, char **env);
void		exit_failure(char *msg, int error);
int			travers_tree(t_ast *root,  t_data *msh);
void		start_job(t_exec_cmd *cmd, t_data *msh);
char		**new_env_arr(t_list *list_env);
int			open_redirect(t_ast *root,  t_data *msh);
void		free_minishell_data(t_data *msh);
void		panic(t_data *msh);

#endif