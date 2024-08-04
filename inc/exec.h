#ifndef EXEC_H
# define EXEC_H

# include "parser.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

int			get_last_status(t_list *env);
void		set_last_status(t_list *list_env, int new_value);
int			start_pipeline(t_ast *root, t_list *list_env);
int			ft_open(char *fname, int mode);
int			ft_close(int fd);
int			ft_execve(t_exec_cmd *cmd, char **env);
void		exit_failure(char *msg);
int			travers_tree(t_ast *root, t_list *list_env);
void		start_job(t_list *list_env, t_exec_cmd *cmd);
void		open_cmd_files(t_exec_cmd *cmd);
char		**new_env_arr(t_list *list_env);

#endif