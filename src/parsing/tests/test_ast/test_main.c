#include "exec.h"
#include "minishell.h"


void	free_minishell(t_lexer *lex, t_parser *parser, t_ast *root)
{
	free_lexer(lex);
	free_parser(parser);
	free_ast(root);
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_lexer 	*lex;
	t_cmd		*root;
	t_list		*env_lst;
	t_parser	*parser;
	char 		*input_str;
	t_env		*n = new_env(get_var_name("?=0"), get_var_value("?=0"), HIDDEN);

	env_lst = new_env_list(env);
	ft_lstadd_back(&env_lst, ft_lstnew(n));
	if (av[1])
		input_str = av[1];
	else
		return 1;
	lex = new_lexer(input_str, env_lst);
	init_list(lex);
	parser = new_parser(lex);
	root = new_ast(parser);
	travers_tree((t_ast *)root, env_lst);
	free_lexer(lex);
	free_parser(parser);
	free_ast((t_ast *)root);
	return (0);
}