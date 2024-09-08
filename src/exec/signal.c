/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoremyk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:47:19 by sgoremyk          #+#    #+#             */
/*   Updated: 2024/09/08 16:56:36 by sgoremyk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include "libft.h"
#include "error.h"

extern int g_exit_code;

static void	sig_handle(int signum);

void	signal_print_newline(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	g_exit_code = FT_SIGINT;
}

void	signal_reset_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_code = FT_SIGINT;
}

void	set_signals_interactive(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	act.sa_handler = &signal_reset_prompt;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
}

void	set_signals_noninteractive(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
	act.sa_handler = &signal_print_newline;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, NULL);
	//sigaction(SIGQUIT, &act, NULL);
}

void	init_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handle;
	sigaction(SIGINT, &sa, NULL);
	//sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	setup_termios(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void	sig_default(void)
{
    struct sigaction sa_child;

	ft_memset(&sa_child, 0, sizeof(sa_child));
    sigemptyset(&sa_child.sa_mask);
    sa_child.sa_handler = SIG_DFL;
    sa_child.sa_flags = 0;
    if (sigaction(SIGINT, &sa_child, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

static void	sig_handle(int signum)
{
    if (signum == SIGINT)
    {
		printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
		g_exit_code = FT_SIGINT;
    }
	else if (signum == SIGQUIT)
    	rl_redisplay();
}
