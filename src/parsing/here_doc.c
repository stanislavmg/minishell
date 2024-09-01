/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <sgoremyk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:38:45 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/01 15:24:10 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static char	*new_hidden_fname(char *stop_word, t_env *tmp_dir);

char	*here_doc_start(char *stop_word, t_list *env)
{
	int		out_fd;
	char	*new_fname;
	char	*next_str;

	if (!stop_word)
		return (NULL);
	new_fname = new_hidden_fname(stop_word, get_env(env, "TMPDIR"));
	out_fd = ft_open(new_fname, O_RDWR | O_CREAT | O_TRUNC);
	next_str = readline("> ");
	while (next_str && ft_strcmp(stop_word, next_str))
	{
		write(out_fd, next_str, ft_strlen(next_str));
		write(out_fd, "\n", 1);
		free(next_str);
		next_str = readline("> ");
	}
	ft_close(out_fd);
	free(next_str);
	free(stop_word);
	return (new_fname);
}

static char	*new_hidden_fname(char *stop_word, t_env *tmp_dir)
{
	char	*hidden_fname;

	hidden_fname = NULL;
	if (tmp_dir && tmp_dir->value)
		hidden_fname = ft_strjoin(tmp_dir->value, stop_word);
	else
		hidden_fname = ft_strjoin("/tmp/.", stop_word);
	return (hidden_fname);
}
