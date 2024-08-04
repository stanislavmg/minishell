/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:17:49 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "minishell.h"
# include <signal.h>
# include <limits.h>

# define INVALID_COMMAND 2
# define CANT_EXECUTE 126
# define COMMAND_NOT_FOUND 127
# define WRONG_EXIT 128
# define CONTROL_C 130
# define CONTROL_D 131
# define WRONG_CODE 255

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
} t_env;

int		handle_command(char **args, t_env *env, int *fd);
int		handle_cd(char **args, t_env *env, int *fd);
int		handle_env(char **args, t_env *env, int *fd);
int		handle_pwd(char **args, t_env *env, int *fd);
int		handle_export(char **args, t_env *env, int *fd);
int		handele_echo(char **args, t_env *env, int *fd);
int		handle_unset(char **args, t_env *env, int *fd);
int		handle_exit(char **args, t_env *env, int *fd);
int		execute_command(char **args, t_env *env, int *fd);
char	*get_env(char *key, t_env *env);
int		set_env(t_env *env, char *key, char *value);
t_env	*list_new(char *key, char *value);
void	list_add(t_env **lst, t_env *new);
t_env	*list_search(t_env *lst, char *key);
void	list_delete_one(t_env **lst, char *key);
void	list_delete(t_env **lst);
int		ft_list_size(t_env *lst);
char	**ft_first_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
void	free_array(char **arr);

#endif
