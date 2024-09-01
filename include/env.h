/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/09/01 15:21:18 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

/* variables attributes */
# define HIDDEN	0x00 
# define EXPORT 0x01
# define ENV	0x02

# include "types.h"
# include "libft.h"

int		set_env(t_list *list_env, t_var *new_env, int attr);
t_env	*get_env(t_list *list_env, char *key);
t_env	*new_env(char *key, char *value, int attr);
t_list	*new_env_list(char **env);
void	free_env(void *env);
char 	*get_var_name(const char *token_word);
char 	*get_var_value(const char *token_word);

#endif
