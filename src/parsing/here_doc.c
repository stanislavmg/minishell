/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:38:45 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/10 18:24:24 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*new_hidden_fname(int count, char *stop, t_env *tmp_dir);
char		*new_var_name(char *str);
int			is_var_delimeter(int ch);

char	*expand_variable(char **input, char *str, t_list *env)
{
	char	*var_name;
	t_env	*var;

	var_name = new_var_name(str);
	*input += ft_strlen(var_name);
	var = get_env(env, var_name);
	free(var_name);
	if (var)
		var_name = ft_strdup(var->value);
	else
		var_name = NULL;
	return (var_name);
}

void	write_variable(char *input, t_list *env, int fd)
{
	char	*start_var;
	char	*var_value;

	var_value = NULL;
	while (*input)
	{
		start_var = ft_strchr(input, '$');
		if (!start_var)
		{
			write(fd, input, ft_strlen(input));
			input += ft_strlen(input);
		}
		else
		{
			write(fd, input, start_var - input);
			start_var++;
			input = start_var;
			var_value = expand_variable(&input, start_var, env);
			if (var_value)
				write(fd, var_value, ft_strlen(var_value));
			free(var_value);
			start_var = NULL;
		}
	}
}

char	*here_doc_start(char *stop, t_list *env, t_token_type mode)
{
	static int	count = 0;
	int			out_fd;
	char		*new_fname;
	char		*input;

	if (!stop)
		return (NULL);
	count++;
	new_fname = new_hidden_fname(count, stop, get_env(env, "TMPDIR"));
	out_fd = ft_open(new_fname, O_RDWR | O_CREAT | O_TRUNC);
	input = readline("> ");
	while (input && ft_strcmp(stop, input))
	{
		if (mode == EXPAND_HERE_DOC)
			write_variable(input, env, out_fd);
		else
			write(out_fd, input, ft_strlen(input));
		write(out_fd, "\n", 1);
		free(input);
		input = readline("> ");
	}
	ft_close(out_fd);
	free(input);
	free(stop);
	return (new_fname);
}

static char	*new_hidden_fname(int count, char *stop, t_env *tmp_dir)
{
	char	*hidden_fname;
	char	*t;

	t = ft_strjoin(ft_itoa(count), stop);
	hidden_fname = NULL;
	if (tmp_dir && tmp_dir->value)
	{
		hidden_fname = ft_strjoin(tmp_dir->value, t);
	}
	else
		hidden_fname = ft_strjoin("/tmp/", t);
	free(t);
	return (hidden_fname);
}
