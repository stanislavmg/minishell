/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 04:49:46 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:12:28 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

t_env	*list_new(char *key, char *value)
{
	t_env	*lst;

	lst = malloc(sizeof(t_env));
	if (!lst)
		return (NULL);
	lst -> key = key;
	lst -> value = value;
	lst -> next = NULL;
	return (lst);
}

t_env	*list_last(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst -> next)
		lst = lst -> next;
	return (lst);
}

void	list_add(t_env **lst, t_env *new)
{
	t_env	*ps;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ps = list_last(*lst);
	ps -> next = new;
}

t_env	*list_search(t_env *lst, char *key)
{
	while (lst)
	{
		if (ft_strcmp(lst -> key, key) == 0)
			return (lst);
		lst = lst -> next;
	}
	return (NULL);
}

int	ft_list_size(t_env *lst)
{
	int	len;

	len = 0;
	while (lst)
	{
		lst = lst -> next;
		len++;
	}
	return (len);
}
