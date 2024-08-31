/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 19:29:16 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "env.h"
# include <signal.h>
# include <limits.h>
# include <stdio.h>
# include <strings.h>
# include <stdio.h>
# include <ctype.h>

# define INVALID_COMMAND 2
# define CANT_EXECUTE 126
# define COMMAND_NOT_FOUND 127
# define WRONG_EXIT 128
# define CONTROL_C 130
# define CONTROL_D 131
# define WRONG_CODE 255

// builtin commands
int		handle_command(char **args, t_data *msh);
int		handle_cd(char **args, t_list *env);
int		handle_env(char **args, t_list *env);
int		handle_pwd(char **args);
int		handle_export(char **args, t_list *env);
int		handle_echo(char **args);
int		handle_unset(char **args, t_list *env);
int		handle_exit(char **args, t_data *msh);

// env funcs
char	*get_env_value(char *key, t_list *env);
int		set_env_value(t_list *env, char *key, char *value);
void	env_delete(t_list **env, char *key);
int		ft_env_count(t_list *env, int attr);

// export funcs
char	**sort_exports(char **exports);
char	**get_exports(t_list *env, int i);
int		concat_env_value(t_list *env, char *key, char *value);
int		concat_value(t_env *node, char *value, char *new_key);

// utility funcs
char	**ft_first_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
void	free_array(char **arr);
void	ft_print_error(char *command, char *str, char *error);
void	free_minishell_data(t_data *msh);
int		ft_count_args(char **args);
int		free_arr_and_return_failure(char **arr);
int		ft_isnum(char *str);

// cd funcs
int		cd_path(char *path, t_list *env);
int		cd_home(t_list *env);
int		cd_oldpwd(t_list *env);

// exit funcs
int		calc_exit_status(char *arg, t_data *msh);
int		ft_atol(char *str, t_data *msh);

#endif
