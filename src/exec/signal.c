#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

#include "libft.h"
static void	sig_handle(int signum, siginfo_t *info, void *context);

void	init_signals(int i)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sig_handle;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	remove_echo_ctl(void)
{
	struct termios	t;

	if (tcgetattr(0, &t) == 0)
	{
		t.c_lflag &= ~(ECHOCTL);
		tcsetattr(0, TCSANOW, &t);
	}
}

static void	sig_handle(int signum, siginfo_t *info, void *context)
{
	write(1, "\n", 1);
	//rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}