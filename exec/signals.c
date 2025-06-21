
#include "execution.h"

void	handle_sigint(int sig)
{
	(void)sig;
	if (g_signal_status)
	{
		if (g_signal_status != 138)
			g_signal_status = 138;
		return ;
	}
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig; 
	// Exit heredoc input immediately with the signal code stored in g_signal
	exit(g_signal_status);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	if (g_signal_status)
	{
		if (g_signal_status != 130)
			g_signal_status = 130;
		return ;
	}
}
