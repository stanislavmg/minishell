#include "parser.h"

void here_doc_start(void)
{

}

t_var	*new_tvar(const t_word_list *token)
{
	t_var	*new_var;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		return (NULL);
	new_var->type = VARIABLE;
	new_var->key = get_var_name(token->word);
	new_var->value = get_var_value(token->word);
	return (new_var);
}

t_cmd	*assignment_handle(t_parser *parser, t_cmd *root)
{
	t_cmd		*new_tree;
	t_word_list *cur_token;

	if (!parser || !parser->token)
		return (NULL);
	new_tree = NULL;
	cur_token = parser->token;
	new_tree = (t_cmd *)new_tvar(cur_token);
	if (root)
		new_tree = add_tnode(root, new_tree, SEMICOLON);
	return (new_tree);
}

int parse_redirect(t_exec_cmd *new_cmd, t_word_list *token)
{
	if (token->type == HERE_DOC)
		here_doc_start(); // TODO
	add_field_fname(token->type, token->next->word, new_cmd);
	add_field_open_mode(token->type, new_cmd);
	token->word = NULL;
}

t_cmd	*parse_token(t_parser *parser)
{
	t_exec_cmd	*new_cmd;
	t_cmd		*var_assignment_tree;
	t_list		*args;
	t_word_list *cur_token;

	if (!parser || !parser->token)
		return (NULL);
	if (is_cmd_delimeter(parser->token->type))
	{
		printf("minishell: syntax error near unexpected token `%s'", parser->token->word);
		return (NULL);
	}
	var_assignment_tree = NULL;
	cur_token = parser->token;
	args = NULL;
	new_cmd = (t_exec_cmd *)new_exec_cmd();
	if (!new_cmd)
	{
		parser->err = ERR_MEM;
		return (NULL);
	}
	while (cur_token && !is_cmd_delimeter(cur_token->type))
	{
		if (ft_strchr(cur_token->word, '=') && !parser->cmd_start)
			var_assignment_tree = assignment_handle(parser, var_assignment_tree);
		else if (is_redirect(cur_token->type))
		{
			parse_redirect(new_cmd, cur_token);
			cur_token = cur_token->next;
		}
		else if (cur_token->type == STRING && !parser->cmd_start)
		{
			parser->cmd_start = 1;
			ft_lstadd_back(&args, ft_lstnew(cur_token->word));
			cur_token->word = NULL;
		}
		cur_token = cur_token->next;
	}
	if (parser->cmd_start)
	{
		parser->cmd_start = 0;
		add_field_argv(args, new_cmd);
		ft_lstclear(&args, free);
		if (var_assignment_tree)
			var_assignment_tree = add_tnode(var_assignment_tree, (t_cmd *)new_cmd, SEMICOLON);
		else
			var_assignment_tree = (t_cmd *)new_cmd;
	}
	else
		free_cmd(new_cmd);
	parser->token = cur_token;
	return (var_assignment_tree);
}

int add_field_argv(t_list *args, t_exec_cmd *cmd)
{
	char	**argv;
	int		argc;
	int		i;

	if (!args || !cmd)
		return (1);
	i = -1;
	argc = ft_lstsize(args) + 1;
	argv = (char **)malloc(sizeof(char *) * argc);
	if (!argv)
		return (1);
	while (++i < argc - 1)
	{
		argv[i] = (char *)args->content;
		args->content = NULL;
		args = args->next;
	}
	argv[i] = NULL;
	cmd->argv = argv;
	return (0);
}

t_cmd	*add_tnode(t_cmd *left_node, t_cmd *right_node, int type)
{
	t_ast	*root;

	root = (t_ast *)ft_calloc(1, sizeof(t_ast));
	if (!root)
		return (NULL);
	root->left = left_node;
	root->right = right_node;
	root->type = type;
	return ((t_cmd *)root);
}

t_cmd	*build_tree(t_parser *parser)
{
	t_cmd	*root;
	e_token	type;
	
	if (!parser->token)
		return (NULL);
	root = parse_token(parser);
	while (parser->token)
	{
		type = parser->token->type;
		parser->token = parser->token->next;
		if (type == OPEN_BRACKET)
		{
			parser->brackets_count++;
			root = add_tnode(root, build_tree(parser), type);
		}
		else if (type == CLOSED_BRACKET)
		{
			parser->brackets_count--;
			return (root);
		}
		else if (type == AND || type == OR || type == SEMICOLON)
			root = add_tnode(root, build_subtree(parser), type);
		else
			root = add_tnode(root, parse_token(parser), type);
	}
	return (root);
}

t_cmd	*build_subtree(t_parser *parser)
{
	e_token type;
	t_cmd	*root;
	
	root = NULL;
	if (parser->token->type == OPEN_BRACKET)
	{
		parser->token = parser->token->next;
		root = build_tree(parser);
	}
	else
		root = parse_token(parser);
	while (parser->token &&
			parser->token->type != AND &&
			parser->token->type != OR &&
			parser->token->type != SEMICOLON &&
			parser->token->type != CLOSED_BRACKET)
	{
		type = parser->token->type;
		parser->token = parser->token->next;
		root = add_tnode(root, parse_token(parser), type);
	}
	return (root);
}

t_cmd *build_AST(t_parser *parser)
{
	t_cmd		*root;

	root = build_tree(parser);
	if (!root)
		return (NULL);
	if (parser->brackets_count)
		printf("Brackets didn't close\n");
	print_tree((t_ast *)root);
	return (root);
}
