/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amikhush <<marvin@42.fr>>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:11:41 by amikhush          #+#    #+#             */
/*   Updated: 2024/08/19 07:55:35 by amikhush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	**find_exec(char *command, t_list *env)
{
	char	*path;
	char	**bins;
	char	*tmp;
	int		i;

	path = get_env_value("PATH", env);
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

static void	fill_envp_string(char *str, t_list *env)
{
	char	*s;
	int		i;
	int		j;
	t_env	*target;

	i = -1;
	j = 0;
	target = (t_env *)env -> data;
	s = target -> key;
	while (s[++i])
		str[i] = s[i];
	s = target -> value;
	if (s && ft_strlen(s) > 0)
	{
		str[i++] = '=';
		while (s[j])
		str[i++] = s[j++];
		str[i] = '\0';
	}
}

static char	**make_env(t_list *env)
{
	char	**envp;
	int		count;
	int		i;
	t_env	*target;

	i = 0;
	count = ft_env_count(env, ENV);
	envp = malloc(sizeof(char *) * (count + 1));
	while (i < count)
	{
		target = (t_env *)env -> data;
		envp[i] = malloc(sizeof(char) * (ft_strlen(target -> key) 
			+ ft_strlen(target -> value) + 4));
		fill_envp_string(envp[i], env);
		env = env -> next;
		i++;	
	}
	envp[i] = NULL;
	return (envp);
}

static int	exec_command(char **args, t_list *env)
{
	char	**bins;
	char	**command_env;
	int		i;

	i = 0;
	bins = find_exec(args[0], env);
	command_env = make_env(env);
	while (bins[i])
	{
		if ((access(bins[i], X_OK) == 0) 
			&& (execve(bins[i], args, command_env) == 0))
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

int	execute_command(char **args, t_list *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (exec_command(args, env) != 0)
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
			ft_putendl_fd("Execve error\n", STDOUT_FILENO);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
