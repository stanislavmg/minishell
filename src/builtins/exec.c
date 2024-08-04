/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:11:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/04 15:16:47 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"

static char	**find_exec(char *command, t_env *env)
{
	char	*path;
	char	**bins;
	char	*tmp;
	int		i;

	path = get_env("PATH", env);
	if (path == NULL)
	{
		ft_putendl_fd("PATH not found", STDERR_FILENO);
		return (NULL);
	}
	bins = ft_split(path, ':');
	i = 0;
	while (bins[i])
	{
		tmp = ft_strjoin(bins[i], "/");
		free(bins[i]);
		bins[i] = ft_strjoin(tmp, command);
		free(tmp);	
		i++;
	}
	return (bins);
}

static void	fill_envp_string(char *str, t_env *env)
{
	char	*s;
	int		i;
	int		j;

	i = -1;
	j = 0;
	s = env -> key;
	while (s[++i])
		str[i] = s[i];
	s = env -> value;
	if (s && ft_strlen(s) > 0)
	{
		str[i++] = '=';
		while (s[j])
		str[i++] = s[j++];
		str[i] = '\0';
	}
}

static char	**make_env(t_env *env)
{
	char	**envp;
	int		count;
	int		i;

	i = 0;
	count = ft_list_size(env);
	envp = malloc(sizeof(char *) * (count + 1));
	while (i < count)
	{
		envp[i] = malloc(sizeof(char) * (ft_strlen(env->key) + ft_strlen(env->value) + 4));
		fill_envp_string(envp[i], env);
		env = env->next;
		i++;	
	}
	envp[i] = NULL;
	return (envp);
}

static int	exec_command(char **args, t_env *env, int *fd)
{
	char	**bins;
	char	**command_env;
	int		i;

	i = 0;
	bins = find_exec(args[0], env);
	command_env = make_env(env);
	while (bins[i])
	{
		if ((access(bins[i], X_OK) == 0) && (execve(bins[i], args, command_env) == 0))
		{
			free_array(bins);
			free_array(command_env);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	free_array(bins);
	free_array(command_env);
	ft_putendl_fd("Command not found", STDERR_FILENO);
	return (EXIT_SUCCESS);
}

int	execute_command(char **args, t_env *env, int *fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (exec_command(args, env, fd) != 0)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
	{
		ft_putendl_fd("Fork error\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) != EXIT_SUCCESS)
		{
			ft_putendl_fd("Execve error\n", fd[1]);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
