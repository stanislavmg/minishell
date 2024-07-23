# include "env.h"



int		set_env(t_env *env, char *key, char *value)
{
	t_env *search_var;

	if (!env || !key)
		return (1);
	search_var = list_search(env, key);
	if (search_var->value = value)
		return (0);
	free(search_var->value);
	search_var->value = value;
	return (0);
}