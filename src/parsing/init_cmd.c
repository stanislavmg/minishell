#include "parser.h"

t_cmd	*create_cmd(void)
{
	t_exec_cmd	*new_cmd;

	new_cmd = (t_exec_cmd *)ft_calloc(1, sizeof(t_exec_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->type = COMMAND;
	return ((t_cmd *)new_cmd);
}

int	add_field_files(t_word_list *token, t_exec_cmd *cmd)
{
	while (token && !is_cmd_delimeter(token->type) && !is_redirect(token->type))
		token = token->next;
	if (!token || !is_redirect(token->type))
		return (0);
	if (INPUT_TRUNC == token->type)
	{
		cmd->in_fmode = O_RDONLY;
		cmd->in_fname = token->next->word;
	}
	else if (OUTPUT_TRUNC == token->type)
	{
		cmd->out_fmode = O_TRUNC | O_CREAT | O_RDWR;
		cmd->out_fname = token->next->word;
	}
	else if (OUTPUT_ADD == token->type)
	{
		cmd->out_fmode = O_APPEND | O_CREAT | O_RDWR;
		cmd->out_fname = token->next->word;
	}
	else if (HERE_DOC == token->type)
	{
		cmd->in_fmode = HERE_DOC;
		cmd->in_fname = token->next->word;
	}
	else
	token->next->word = NULL;
	return (0);
}

char	**get_path(char *path_env)
{
	char	**path;
	char	*tmp;
	int		i;

	i = -1;
	path = ft_split(path_env, ':');
	if (!path)
		return (NULL);
	while (path[++i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			tmp = ft_strjoin(path[i], "/");
			if (!tmp)
			{
				free_arr(path);
				return (NULL);
			}
			free(path[i]);
			path[i] = tmp;
		}
	}
	return (path);
}

char	*parsing_path(char **path_env, char *cmd_name)
{
	int		i;
	char	*cmd_path;

	i = 0;
	if (!path_env || !*path_env || !cmd_name)
		return (NULL);
	cmd_path = (char *)malloc(sizeof(char) * ft_strlen(cmd_name) + 1);
	if (!cmd_path)
		return (NULL);
	ft_strlcpy(cmd_path, cmd_name, ft_strlen(cmd_name) + 1);
	while (path_env[i])
	{
		if (!access(cmd_path, F_OK))
			return (cmd_path);
		free(cmd_path);
		cmd_path = ft_strjoin(path_env[i], cmd_name);
		if (!cmd_path)
			return (NULL);
		i++;
	}
	free(cmd_path);
	return (NULL);
}