# include "parser.h"

int here_doc_start(t_token *hd_token)
{
	int		out_fd;
	char	*next_str;
	char	*hidden_fname;
	char	*stop_word;

	hidden_fname = NULL;
	if (!hd_token || hd_token->type != HERE_DOC)
		return (1);
	stop_word = hd_token->word;
	hidden_fname = ft_strjoin("/tmp/.", hd_token->word);
	out_fd = ft_open(hidden_fname, O_RDWR | O_CREAT | O_TRUNC);
	next_str = readline("> ");
	while (strcmp(stop_word, next_str))
	{
		write(out_fd, next_str, ft_strlen(next_str));
		write(out_fd, "\n", 1);
		free(next_str);
		next_str = readline("> ");
	}
	ft_close(out_fd);
	free(next_str);
	free(stop_word);
	hd_token->word = hidden_fname;
	return (0);
}