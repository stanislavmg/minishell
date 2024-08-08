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

	(void)ac;
	(void)av;
	env_lst = new_env_list(env);
	ft_lstadd_back(&env_lst, ft_lstnew(n));
	while (1)
	{
		input_str = readline(PROMT);
		if (!strcmp(input_str, "exit"))
		{
			free(input_str);
			ft_lstclear(&env_lst, free_env);
			return (0);
		}
		lex = new_lexer(input_str, env_lst);
		init_list(lex);
		print_tokens(lex->tokens);
		parser = new_parser(lex);
		root = new_ast(parser);
		print_tree((t_ast *)root);
		if (!parser->err)
		{
			travers_tree((t_ast *)root, env_lst);
		}
		else
		{
			if (parser->cur_token_pos)
				print_msh_err(parser->cur_token_pos->data);
			else
				print_msh_err(ft_lstlast(lex->tokens)->data);
		}
		free_lexer(lex);
		free_parser(parser); 
		free_ast((t_ast *)root);
	}
	return (0);
}