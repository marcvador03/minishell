/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2024/12/01 19:03:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    signal_handler_main(int sig)
{
    if (sig == SIGINT)
    {
		write(STDERR_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
	/*else if (sig == SIGSTOP)
    {
		write(STDERR_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }*/
	else if (sig == SIGTERM)
    {
		write(STDERR_FILENO, "\n", 1);
        exit(0);
    }
}

void    control_c_heredoc(int sig)
{
    if (sig == SIGINT)
    {
        close(STDIN_FILENO);
        write(STDERR_FILENO, "\n", 1);
    }
}
