/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/13 19:10:59 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "minishell.h"
# include <signal.h>
# include <limits.h>
# include <stdio.h>
# include <strings.h>
# include <stdio.h>
# include <ctype.h>
# include "env.h"
# include "libft.h"


# define INVALID_COMMAND 2
# define CANT_EXECUTE 126
# define COMMAND_NOT_FOUND 127
# define WRONG_EXIT 128
# define CONTROL_C 130
# define CONTROL_D 131
# define WRONG_CODE 255

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// } t_env;

int		handle_command(char **args, t_list *env);
int		handle_cd(char **args, t_list *env);
int		handle_env(char **args, t_list *env);
int		handle_pwd(char **args);
int		handle_export(char **args, t_list *env);
int		handele_echo(char **args);
int		handle_unset(char **args, t_list *env);
int		handle_exit(char **args);
int		execute_command(char **args, t_list *env);
char	*get_env_value(char *key, t_list *env);
int		set_env_value(t_list *env, char *key, char *value);
void	env_delete(t_list *env, char *key);
char	**ft_first_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
void	free_array(char **arr);
int		ft_env_count(t_list *env, int attr);
// void	signal_init(void);

#endif
