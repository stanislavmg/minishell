/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:30:42 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/08/05 16:17:10 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "env.h"

t_list	*new_env_list(char **env)
{
	char	*var_name;
	char	*var_value;
	t_list	*list_env;
	t_env	*new_var;

	list_env = NULL;
	if (!env)
		return (NULL);
	while (*env)
	{
		var_name = get_var_name(*env);
		if (!var_name)
			return (list_env);
		var_value = get_var_value(*env);
		if (!var_value)
		{
			free(var_name);
			return (list_env);
		}
		new_var = new_env(var_name, var_value, EXPORT | ENV);
		ft_lstadd_back(&list_env, ft_lstnew(new_var));
		env++;
	}
	return (list_env);
}

int		set_env(t_list *list_env, t_var *var, int attr)
{
	t_env *target;

	if (!list_env || !var)
		return (1);
	target = get_env(list_env, var->key);
	if (!target)
	{
		target = new_env(var->key, var->value, attr);
		ft_lstadd_back(&list_env, ft_lstnew(target));
		var->key = NULL;
		var->value = NULL;
	}
	else if (target->value == var->value)
		return (0);
	else
	{
		free(target->value);
		target->value = var->value;
		var->value = NULL;
	}
	return (0);
}