#include "../../inc/parser.h"

t_cmd	*create_cmd(void)
{
	t_exec_cmd	*new_cmd;

	new_cmd = (t_exec_cmd *)ft_calloc(1, sizeof(t_exec_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->file_out = 1;
	new_cmd->type = COMMAND;
	return ((t_cmd *)new_cmd);
}

int	add_field_fd(t_word_list *token, t_exec_cmd *cmd)
{
	while (token && !is_cmd_delimeter(token->type) && !is_redirect(token->type))
		token = token->next;
	if (!token || !is_redirect(token->type))
		return (0);
	if (INPUT_TRUNC == token->type)
	{
		if (access(token->word, F_OK | R_OK))
			perror(token->word);
		cmd->file_in = open(token->next->word, O_RDONLY);
	}
	else if (OUTPUT_TRUNC == token->type)
	{
		if (access(token->word, W_OK))
			perror(token->word);
		cmd->file_out = open(token->next->word, O_TRUNC | O_CREAT | O_RDWR, 0644);
	}
	else if (OUTPUT_ADD == token->type)
	{
		if (access(token->word, W_OK))
			perror(token->word);
		cmd->file_out = open(token->next->word, O_APPEND | O_CREAT | O_RDWR, 0644);
	}
	//else if (HERE_DOC == token->type)
		//start_heredoc(); // TODO
	else
		printf("Incorrect type of token\n");
	return (0);
}