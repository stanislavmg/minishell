/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:30:45 by amikhush          #+#    #+#             */
/*   Updated: 2024/07/23 11:01:55 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
} t_env;

int		set_env(t_env *env, char *key, char *value);
t_env	*list_new(char *key, char *value);
void	list_add(t_env **lst, t_env *new);
t_env	*list_search(t_env *lst, char *key);
void	list_delete_one(t_env *lst, char *key);
void	list_delete(t_env **lst);

#endif
