/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/14 12:34:15 by sgoremyk         ###   ########.fr       */
=======
/*   Updated: 2024/08/06 11:10:17 by sgoremyk         ###   ########.fr       */
>>>>>>> origin/parser
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/* variables attributes */
# define EXPORT 0x01
# define ENV	0x02
# define HIDDEN	0x03

# include "libft.h"
# include "minishell.h"
# include <string.h>

<<<<<<< HEAD
typedef struct s_env
{
	char	*key;
	char	*value;
	int 	attr;
} 	t_env;

//int		set_env(t_list *list_env, t_var *new_env, int attr);
=======
int		set_env(t_list *list_env, t_var *new_env, int attr);
>>>>>>> origin/parser
t_env	*new_env(char *key, char *value, int attr);
void	free_env(void *env);
t_env	*get_env(t_list *list_env, char *key);
char 	*get_var_name(const char *token_word);
char 	*get_var_value(const char *token_word);
t_list	*new_env_list(char **env);

#endif
