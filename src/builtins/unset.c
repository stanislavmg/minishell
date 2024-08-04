/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:17:04 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

int	handle_unset(char **args, t_env *env, int *fd)
{
	t_env	*node;

	if (!args[1])
		return (EXIT_SUCCESS);
	if (node)
		list_delete_one(&env, args[1]);
	return (EXIT_SUCCESS);
}
