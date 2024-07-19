#include "../../inc/parser.h"

static int	is_cmd_delimeter(e_token type)
{ 
	return (AND == type || PIPE == type ||
			OR == type || SEMICOLON == type);
}

static int	is_redirect(e_token type)
{
	return (HERE_DOC == type || INPUT_TRUNC == type ||
			OUTPUT_ADD == type || OUTPUT_TRUNC == type);
}

int	check_syntax(t_lexer *lex)
{
	t_word_list *tokens;

	tokens = lex->tokens;
	while (tokens)
	{
		if (is_cmd_delimeter(tokens->type) && is_cmd_delimeter(tokens->next->type))
		{
			lex->err = ERR_SYNTAX;
			return (1);
		}
		tokens = tokens->next;
	}
	return (0);
}

char **split_var(char *str)
{
	char	**res;
	char	*start_value;
	int		len_value;
	int		len_name;

	res = (char **)malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	start_value = ft_strchr(str, '=');
	len_value = ft_strlen(start_value + 1);
	len_name = start_value - str;
	res[0] = (char *)malloc(sizeof(char) * len_name + 1);
	if (!res[0])
	{
		free(res);
		return (NULL);
	}
	res[1] = (char *)malloc(sizeof(char) * len_value + 1);
	if (!res[1])
	{
		free(res[0]);
		free(res);
		return (NULL);
	}
	ft_strncpy(res[0], str, len_name);
	ft_strncpy(res[1], start_value + 1, len_value);
	res[2] = NULL;
	return (res);
}

int	insert_variable(t_lexer *lex, t_word_list *token)
{
	char **new_var;

	new_var = split_var(token->word);
	if (!new_var)
	{
		lex->err = ERR_MEM;
		return (1);
	}
	//add_var(); // TO DO function which insert var in one-link list
	return (0);
}

t_cmd	*add_tnode(t_cmd *left_node, t_cmd *right_node, int type)
{
	t_pipecmd	*res;

	res = (t_pipecmd *)ft_calloc(1, sizeof(t_pipecmd));
	if (!res)
		return (NULL);
	res->left = left_node;
	res->right = right_node;
	res->type = type;
	return ((t_cmd *)res);
}

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
		cmd->file_out = open(token->word, O_TRUNC | O_CREAT | O_RDWR, 0644);
	}
	else if (OUTPUT_ADD == token->type)
	{
		if (access(token->word, W_OK))
			perror(token->word);
		cmd->file_out = open(token->word, O_APPEND | O_CREAT | O_RDWR, 0644);
	}
	//else if (HERE_DOC == token->type)
		//start_heredoc(); // TODO
	else
		printf("Incorrect type of token\n");
	return (0);
}

char	**add_field_argv(t_word_list *tokens, int argc)
{
	char	**argv;
	int		i;

	i = 0;
	argv = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	while (tokens && !is_cmd_delimeter(tokens->type))
	{
		argv[i] = tokens->word;
		tokens->word = NULL;
		tokens = tokens->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

t_cmd	*parseline(t_lexer *lex)
{
	t_exec_cmd	*new_cmd;
	t_word_list *cur_token;
	int argc;

	argc = 0;
	cur_token = lex->token_pos;
	if (!lex->token_pos)
		return (NULL);
	new_cmd = (t_exec_cmd *)create_cmd();
	if (!new_cmd)
	{
		lex->err = ERR_MEM;
		return (NULL);
	}
	while (cur_token && !is_cmd_delimeter(cur_token->type))
	{
		// if (new_cmd && ft_strchr(cur_token->word, '='))
		// {
		// 	insert_variable(lex, cur_token);
		// 	lex->token_pos = lex->token_pos->next;
		// }
		if (is_redirect(cur_token->type))
		{
			add_field_fd(lex->token_pos, new_cmd);
			cur_token = cur_token->next;
			lex->token_pos = cur_token;
		}
		else if (cur_token->type == STRING)
			argc++;
		cur_token = cur_token->next;
	}
	new_cmd->argv = add_field_argv(lex->token_pos, argc);
	lex->token_pos = cur_token;
	return ((t_cmd *)new_cmd);
}

t_cmd *build_AST(t_lexer *lex)
{
	t_cmd 			*new_node;
	t_pipecmd		*root;
	e_token			type;

	new_node = NULL;
	if (check_syntax(lex))
		return (NULL);
	new_node = parseline(lex);
	root = new_node;
	if (lex->token_pos && is_cmd_delimeter(lex->token_pos->type))
	{
		type = lex->token_pos->type;
		lex->token_pos = lex->token_pos->next;
		root = add_tnode(new_node, parseline(lex), type);
	}
	while(lex->token_pos && is_cmd_delimeter(lex->token_pos->type))
	{
		type = lex->token_pos->type;
		lex->token_pos = lex->token_pos->next;
		if (lex->token_pos->type == AND || lex->token_pos->type == OR)
		{
			root = add_tnode(root, parseline(lex), type);
		}
		else
			root = add_tnode(root, parseline(lex), type);
	}
	return (root);
}

// void print_tree(t_pipecmd *root) {
//     if (root != NULL) {
// 		if (root->type == COMMAND)
// 		{
//        		printf("%s\n", ((t_exec_cmd *)root)->argv[0]);
// 			printf("type of node: %s\n", get_type(root->type));
// 			return ;
// 		}
// 		printf("type of node: %s\n", get_type(root->type));
// 		if (is_cmd_delimeter(root->type) && root->left)
// 		{
// 			printf("left node = ");
//         	print_tree((t_pipecmd *)root->left);
// 		}
// 		if (is_cmd_delimeter(root->type) && root->right)
// 		{
// 			printf("right node = ");
//         	print_tree((t_pipecmd *)root->right);
// 		}
//     }
// }

void print_tree(t_pipecmd *root) {
    if (root != NULL) {
        if (root->type == COMMAND) {
            printf("%s\n", ((t_exec_cmd *)root)->argv[0]);
            printf("type of node: %s\n", get_type(root->type));
            return;
        }
        printf("type of node: %s\n", get_type(root->type));
        if (root->left != NULL) {
            printf("left node = ");
            print_tree(root->left);
        }
        if (root->right != NULL) {
            printf("right node = ");
            print_tree(root->right);
        }
    }
}
