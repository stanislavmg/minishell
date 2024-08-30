/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:59:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/29 17:59:10 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	g_exit_code;

int	get_exit_code(void)
{
	return (g_exit_code);
}

void	set_exit_code(t_list *list_env, int new_value)
{
	t_env	*last_status;

	g_exit_code = new_value;
	last_status = NULL;
	if (!list_env)
		return ;
	last_status = get_env(list_env, "?");
	if (!last_status)
		return ;
	free(last_status->value);
	last_status->value = ft_itoa(new_value);
}
