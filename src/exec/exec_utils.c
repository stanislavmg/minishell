#include "exec.h"

void	exit_failure(char *msg)
{
	printf("minishell: ");
	perror(msg);
	exit(errno);
}

int	get_last_status(t_list *list_env)
{
	t_env	*last_status;

	if (!list_env)
		return (1);
	last_status = search_env(list_env, "?");
	if (!last_status)
		return (1);
	return (ft_atoi(last_status->value));
}

void	set_last_status(t_list *list_env, int new_value)
{
	t_env	*last_status;

	last_status = NULL;
	if (!list_env)
		return ;
	last_status = search_env(list_env, "?");
	if (!last_status)
		return ;
	free(last_status->value);
	last_status->value = ft_itoa(new_value);
}