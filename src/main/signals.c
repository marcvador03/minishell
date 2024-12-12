/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2024/12/12 19:16:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	signal_handler_main(int sig)
{
	if (sig == SIGINT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = sig + 128;
	}
	/*else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}*/
	return ;
}

static void	signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		//kill(0, SIGINT);
		rl_replace_line("", 0);
		rl_on_new_line();
		g_status = sig + 128;
	}
	else if (sig == SIGQUIT)
	{
		//kill(0, SIGQUIT);
		g_status = sig + 128;
		printf("Quit: %d\n", g_status);
	}
	return ;
}

static void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		g_status = sig + 128;
		exit(g_status);
	}
	else if (sig == SIGQUIT)
	{
		g_status = sig + 128;
		printf("Quit: %d\n", g_status);
		exit(g_status);
	}
	return ;
}

void	init_signal(int pid, int hd)
{
	struct sigaction	sig;

	sigemptyset(&sig.sa_mask);
	signal(SIGQUIT, SIG_IGN);
	sig.sa_flags = 0;
	if (pid == 0 && hd == 0)
	{
		sig.sa_handler = &signal_handler_child;
		signal(SIGQUIT, SIG_DFL);
		sigaction(SIGQUIT, &sig, NULL);
	}
	else if (pid == 1 && hd == 0)
		sig.sa_handler = &signal_handler_main;
	else if (pid == 0 && hd == 1)
		sig.sa_handler = &signal_handler_heredoc;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
}
