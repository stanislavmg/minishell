/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/05 18:09:25 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

int	handle_unset(char **args, t_env *env, int *fd)
{
	t_env	*node;

	if (!args[1]) //SEGFAULT
		return (EXIT_SUCCESS);
	if (node)
		list_delete_one(&env, args[1]);
	return (EXIT_SUCCESS);
}
