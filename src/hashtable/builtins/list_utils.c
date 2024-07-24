/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 04:49:46 by amikhush          #+#    #+#             */
/*   Updated: 2024/07/24 08:15:21 by amikhush         ###   ########.fr       */
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

static void	free_node(t_env *node)
{
	free(node -> key);
	free(node -> value);
	free(node);
}

void	list_delete_one(t_env **lst, char *key)
{
	t_env	*temp;
	t_env	*prev;

	if (!lst || !*lst || !key)
		return ;
	temp = *lst;
	prev = NULL;
	if (ft_strcmp(temp -> key, key) == 0)
	{
		*lst = temp -> next;
		free_node(temp);
		return ;
	}
	while (temp != NULL && ft_strcmp(temp -> key, key) != 0)
	{
		prev = temp;
		temp = temp -> next;
	}
	if (temp == NULL)
		return ;
	prev -> next = temp -> next;
	free_node(temp);
}

void	list_delete(t_env **lst)
{
	t_env	*t;

	if (!lst || !*lst)
		return ;
	while (*lst)
	{
		t = (*lst) -> next;
		free((*lst) -> key);
		free((*lst) -> value);
		free(*lst);
		*lst = t;
	}
	*lst = NULL;
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
