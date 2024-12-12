/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2024/12/12 18:31:30 by mfleury          ###   ########.fr       */
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
		rl_replace_line("", 0);
		rl_on_new_line();
		g_status = sig + 128;
	}
	else if (sig == SIGQUIT)
	{
		g_status = sig + 128;
		printf("Quit: %d\n", g_status);
	}
	return ;
}

void	init_signal(int pid)
{
	struct sigaction	sig;

	//sigemptyset(&sig.sa_mask);
	signal(SIGQUIT, SIG_IGN);
	sig.sa_flags = 0;
	if (pid == 0)
	{
		sig.sa_handler = &signal_handler_child;
		signal(SIGQUIT, SIG_DFL);
		sigaction(SIGQUIT, &sig, NULL);
	}
	else if (pid == 1)
	{
		sig.sa_handler = &signal_handler_main;
	//	sigaddset(&sig.sa_mask, SIGQUIT);
	}
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
}
