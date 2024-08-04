/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_delete.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 11:01:01 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:12:58 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

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
		free_node(*lst);
		*lst = t;
	}
	*lst = NULL;
}
