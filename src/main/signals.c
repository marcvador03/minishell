/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2025/01/06 17:17:13 by mfleury          ###   ########.fr       */
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
	return ;
}

static void	signal_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		g_status = sig + 128;
	}
	else if (sig == SIGQUIT)
	{
		write(STDIN_FILENO, "\n", 1);
		rl_on_new_line();
		g_status = sig + 128;
		ft_putstr_fd("Quit: ", STDERR_FILENO);
		ft_putnbr_fd(g_status, STDERR_FILENO);
	}
	return ;
}

static void	signal_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_status = sig + 128;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		close(STDIN_FILENO);
		write(STDERR_FILENO, "\n", 1);
		exit(g_status);
	}
	else if (sig == SIGQUIT)
	{
		g_status = sig + 128;
		ft_putstr_fd("Quitc: ", STDERR_FILENO);
		ft_putnbr_fd(g_status, STDERR_FILENO);
		write(STDIN_FILENO, "\n", 1);
		exit(g_status);
	}
	return ;
}

void	init_signal(int pid, int hd)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	if (pid == 0 && hd == 0)
	{
		signal(SIGQUIT, signal_handler_child);
		signal(SIGINT, signal_handler_child);
		signal(SIGTERM, signal_handler_child);
	}
	else if (pid == 1 && hd == 0)
	{
		signal(SIGINT, signal_handler_main);
		signal(SIGTERM, signal_handler_main);
	}
	else if (pid == 0 && hd == 1)
	{
		signal(SIGQUIT, signal_handler_heredoc);
		signal(SIGINT, signal_handler_heredoc);
		signal(SIGTERM, signal_handler_heredoc);
	}
}
