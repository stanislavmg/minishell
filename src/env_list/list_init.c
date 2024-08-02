# include "env.h"

t_env	*create_envlist(char **env)
{ 
	int	i;
	char	*var_name;
	char	*var_value;
	t_env	*list_env;

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
		list_add(&list_env, list_new(var_name, var_value));
		i++;
	}
	return (list_env);
}

int		set_env(t_env *env, char *key, char *value)
{
	t_env *search_var;

	if (!env || !key)
		return (1);
	search_var = list_search(env, key);
	if (!search_var)
	{
		list_add(&env, list_new(key, value));
		return (0);
	}
	if (search_var->value == value)
		return (0);
	free(search_var->value);
	search_var->value = value;
	return (0);
}