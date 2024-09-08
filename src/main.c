/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:45:34 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/08 16:58:10 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "types.h"

extern int	g_exit_code;

t_data	*new_msh_data(void)
{
	t_data	*msh;

	msh = (t_data *)ft_calloc(1, sizeof(t_data));
	if (!msh)
	{
		perror("minishell: ");
		exit(ERR_MEM);
	}
	return (msh);
}

t_cmd	*init_msh_data(t_list *env, char *input)
{
	t_list		*tokens;
	t_parser	*parser;
	t_cmd		*ast;

	tokens = new_token_list(env, input);
	if (!tokens)
		return (NULL);
	//print_tokens(tokens);
	parser = new_parser(tokens, env);
	ast = new_ast(parser);
	//print_tree(ast);
	if (parser->err)
	{
		if (parser->cur_token_pos)
			print_msh_err(((t_token *)parser->cur_token_pos->data)->word);
		else
			print_msh_err("newline");
		free_ast((t_ast *)ast);
		ast = NULL;
		set_exit_code(env, 2);
	}
	ft_lstclear(&tokens, free_token);
	free(parser);
	return (ast);
}

int	is_contain_alpha(const char *str)
{
	while (*str)
	{
		if (ft_isalpha(*str))
			return (1);
		str++;
	}
	return (0);
}

void	set_std_val(t_list *env)
{
	t_env	*shell_lvl;
	int		tmp_value;

	if (!env)
		return ;
	set_exit_code(env, 0);
	set_env_value(env, ft_strdup("?"), ft_strdup("0"));
	get_env(env, "?")->attr = HIDDEN;
	shell_lvl = get_env(env, "SHLVL");
	if (!shell_lvl || !*shell_lvl->value || is_contain_alpha(shell_lvl->value))
	{
		free(shell_lvl->value);
		shell_lvl->value = NULL;
		set_env_value(env, ft_strdup("SHLVL"), ft_strdup("1"));
	}
	else
	{
		tmp_value = ft_atoi(shell_lvl->value) + 1;
		free(shell_lvl->value);
		shell_lvl->value = ft_itoa(tmp_value);
	}
}

void set_signals_interactive(void);
void set_signals_noninteractive(void);

int	main(int ac, char **av, char **env)
{
	t_data	*msh;
	char	*input;

	(void)av;
	if (ac > 1)
		return (1);
	msh = new_msh_data();
	msh->env = new_env_list(env);
	set_std_val(msh->env);
	setup_termios();
	while (1)
	{
		set_signals_interactive();
		input = readline(PROMT);
		set_signals_noninteractive();
		if (get_exit_code() == FT_SIGINT)
			set_exit_code(msh->env, FT_SIGINT);
		if (!input)
			break ;
		msh->root = (t_ast *)init_msh_data(msh->env, input);
		if (msh->root)
			travers_tree((t_ast *)msh->root, msh);
		ft_waitpid(msh);
		free_ast(msh->root);
	}
	ft_lstclear(&msh->env, free_env);
	free(input);
	free(msh);
	return (g_exit_code);
}
