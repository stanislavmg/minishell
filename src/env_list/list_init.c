# include "env.h"



char **split_var(char *str)
{
	char	**res;
	char	*start_value;
	int		len_value;
	int		len_name;

	res = (char **)malloc(sizeof(char *) * 2);
	if (!res)
		return (NULL);
	start_value = ft_strchr(str, '=');
	len_value = ft_strlen(start_value + 1);
	len_name = start_value - str;
	res[0] = (char *)malloc(sizeof(char) * len_name + 1);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	res[1] = (char *)malloc(sizeof(char) * len_value + 1);
	if (!res[1])
	{
		free(res[0]);
		free(res);
		return (NULL);
	}
	ft_strncpy(res[0], str, len_name);
	ft_strncpy(res[1], start_value + 1, len_value);
	return (res);
}

t_env	*create_envlist(char **env)
{ 
	int	i;
	char	**var;
	t_env	*list_env;

	i = 0;
	list_env = NULL;
	while (env[i])
	{
		var = split_var(env[i]);
		list_add(&list_env, list_new(var[0], var[1]));
		free(var);
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