# include "env.h"

t_list	*new_env_list(char **env)
{
	int		i;
	char	*var_name;
	char	*var_value;
	t_list	*list_env;
	t_env	*new_var;

	i = 0;
	list_env = NULL;
	while (env[i])
	{
		var_name = get_var_name(env[i]);
		if (!var_name)
			return (list_env);
		var_value = get_var_value(env[i]);
		if (!var_value)
		{
			free(var_name);
			return (list_env);
		}
		new_var = new_env(var_name, var_value, EXPORT | ENV);
		ft_lstadd_back(&list_env, ft_lstnew(new_var));
		i++;
	}
	return (list_env);
}

int		set_env(t_list *list_env, t_var *var, int attr)
{
	t_env *target;

	if (!list_env || !var)
		return (1);
	target = search_env(list_env, var->key);
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