# include "env.h"

char *get_var_name(const char *token_word)
{
	char	*var_name;
	char	*end_var_name;
	size_t	len_var_name;

	var_name = NULL;
	end_var_name = ft_strchr(token_word, '=');
	len_var_name = end_var_name - token_word;
	var_name = (char *)malloc(sizeof(char) * len_var_name + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, token_word, len_var_name + 1);
	return (var_name);
}

char *get_var_value(const char *token_word)
{
	char	*var_value;
	char	*start_var_value;
	size_t	len_var_value;

	var_value = NULL;
	start_var_value = ft_strchr(token_word, '=') + 1;
	len_var_value = ft_strlen(start_var_value);
	var_value = (char *)malloc(sizeof(char) * len_var_value + 1);
	if (!var_value)
		return (NULL);
	ft_strlcpy(var_value, start_var_value, len_var_value + 1);
	return (var_value);
}

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