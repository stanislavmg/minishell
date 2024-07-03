/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:53:41 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/02/01 16:53:43 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*ps;

	ps = malloc(sizeof(t_list));
	if (!ps)
		return (NULL);
	ps -> content = content;
	ps -> next = NULL;
	return (ps);
}
