/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 12:26:49 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "parser.h"
# include "builtins.h"
# include <sys/types.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/stat.h>

void	init_signals(int i);
void	remove_echo_ctl(void);

/* pipeline */
int			initialize_pipeline(t_ast *root, t_data *msh);
void		setup_pipeline(t_ast *root, t_data *msh);
void		create_pipeline(t_data *msh, t_exec_cmd *first);
void		output_to_stdout(void);

/* redirect */
int			open_redirect(t_ast *root, t_redir *rfile, t_data *msh);
int			check_file_permission(char *fname, int mode);

/* exit code */
int			get_exit_code(void);
void		set_exit_code(t_list *list_env, int new_value);

/* travers tree and execution command */
int			travers_tree(t_ast *root,  t_data *msh);
void		start_job(t_exec_cmd *cmd, t_data *msh);
char		**new_env_arr(t_list *list_env);
int			ft_execve(t_exec_cmd *cmd, char **env);

/* helpers */
int			is_builtin(const char *cmd);
int			is_logic_operator(e_token type);

void		panic(t_data *msh);
void		free_minishell_data(t_data *msh);
void		ft_waitpid(t_data *msh);
void		exit_failure(char *msg, int error);
void		print_err(char *sender, char *msg);
void		record_pid(t_data *msh, pid_t pid);

void	check_cmd_permission(t_exec_cmd *cmd);
#endif