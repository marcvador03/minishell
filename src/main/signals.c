/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2024/12/02 13:23:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void    signal_handler_main(int sig)
{
    if (sig == SIGINT)
    {
		write(STDIN_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
	return ;
}

static void    signal_handler_child(int sig)
{
    if (sig == SIGINT)
    {
		rl_replace_line("", 0);
        rl_on_new_line();
	}
	else if (sig == SIGQUIT)
	{
		//ioctl(STDIN_FILENO, TIOCSTI, '\n');
		//write(STDIN_FILENO, "Quit: ", 7);
		printf("Quit: %d\n", g_status);
	}
	return ;
}
void    control_c_heredoc(int sig)
{
    if (sig == SIGINT)
    {
        close(STDIN_FILENO);
        write(STDERR_FILENO, "\n", 1);
    }
}

void init_signal(int pid)
{
	struct sigaction	sig;
	
	ft_bzero(&sig, sizeof(sig));
	//sig.sa_flags = SA_RESTART;
	if (pid == 0)
	{
		sig.sa_handler = &signal_handler_child;
		signal(SIGQUIT, SIG_DFL);
		sigaction(SIGQUIT, &sig, NULL);
	}
	else if (pid == 1)
	{
		sig.sa_handler = &signal_handler_main;
		signal(SIGQUIT, SIG_IGN);
	}
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
}
