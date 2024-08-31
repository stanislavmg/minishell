/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 08:32:24 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/31 06:57:35 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_isnum(char *str)
{
	if (!str)
		return (EXIT_FAILURE);
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (EXIT_SUCCESS);
		str++;
	}
	return (EXIT_FAILURE);
}

int	calc_exit_status(char *arg, t_data *msh)
{
	int	exit_status;

	exit_status = ft_atol(arg, msh);
	exit_status = exit_status % 256;
	if (exit_status < 0)
		exit_status += 256;
	return (exit_status);
}
