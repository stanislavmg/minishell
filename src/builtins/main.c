#include "builtins.h"

int main(int ac, char **av, char **env)
{
	t_list *env_lst;
	pid_t ps;

	env_lst = new_env_list(env);
	ps = fork();
	if (!ps)
		handle_command(av + 1, env_lst);
	av[2] = NULL;
	wait(NULL);
	handle_command(av + 1, env_lst);
	return (0);
}