/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:38:35 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/30 18:10:31 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	remove_duplicate(t_list **redirects)
{
	t_redir	*r_token;
	t_list	*tmp;
	t_list	*prev;

	if (!redirects)
		return ;
	tmp = *redirects;
	prev = NULL;
	while (tmp)
	{
		r_token = tmp->data;
		if (HERE_DOC == r_token->type)
		{
			if (!prev)
				*redirects = tmp->next;
			else
				prev->next = tmp->next;
			unlink(r_token->fname);
			free(r_token->fname);
			ft_lstdelone(tmp, free);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
