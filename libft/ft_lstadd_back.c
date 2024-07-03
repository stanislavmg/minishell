/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:51:49 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 17:46:41 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*ps;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	ps = ft_lstlast(*lst);
	ps -> next = new;
}
