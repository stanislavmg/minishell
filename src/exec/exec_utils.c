#include "exec.h"

void	exit_failure(char *msg)
{
	printf("minishell: ");
	perror(msg);
	exit(EXIT_FAILURE);
}

t_exec_data	*new_exec_data(char **env_arr, t_env *env_lst)
{
	t_exec_data *new_data;

	new_data = (t_exec_data *)ft_calloc(1, sizeof(t_exec_data));
	if (!new_data)
		return (NULL);
	new_data->envp = env_arr;
	new_data->list_env = env_lst;
	new_data->out = 1;
	return (new_data);
}

int	get_last_status(const t_env *env)
{
	t_env	*last_status;

	if (!env)
		return (1);
	last_status = list_search(env, "?");
	if (!last_status)
		return (1);
	return (ft_atoi(last_status->value));
}

void	set_last_status(t_env *env, int new_value)
{
	t_env	*last_status;

	if (!env)
		return ;
	last_status = list_search(env, "?");
	if (!last_status)
		return ;
	free(last_status->value);
	last_status->value = ft_itoa(new_value);
}