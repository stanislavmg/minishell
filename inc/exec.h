#ifndef EXEC_H
# define EXEC_H

# include "parser.h"

typedef struct s_exec_data
{
	t_ast		*cmd_tree;
	char        **envp;
	t_env       *list_env;
	t_exec_cmd  *cur_cmd;
	int         in;
	int         out;
	int         flag;
} t_exec_data;

int			get_last_status(const t_env *env);
void		set_last_status(t_env *env, int new_value);
void		start_job(t_exec_data *exec_info, t_exec_cmd *cmd);
int			ft_open(char *fname, int mode);
int			ft_close(int fd);
int			ft_execve(t_exec_cmd *cmd, int in, int out);
int			travers_tree(t_ast *root, t_exec_data *exec_info);
t_exec_data	*new_exec_data(char **env_arr, t_env *env_lst);
void		exit_failure(char *msg);
void		add_var(t_var *var, t_exec_data *exec_info);

#endif