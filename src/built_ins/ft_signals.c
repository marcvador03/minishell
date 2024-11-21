/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_singal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:19:54 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 09:19:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    control_c(int sig)
{
    if (sig == SIGINT)
    {
        write(STDERR_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
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

void    control_d(int sig)
{
    if (sig == SIGTERM)
    {
        write(STDERR_FILENO, "\n", 1);
        exit(0);
    }
}