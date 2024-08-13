/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:38:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/13 19:02:35 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_unset(char **args, t_list *env)
{
	t_env	*node;

	if (!args || !env)
		return (EXIT_FAILURE);
	if (!args[1])
		return (EXIT_SUCCESS);
	node = get_env(env, args[1]);
	if (node)
		env_delete(env, args[1]);
	return (EXIT_SUCCESS);
}
