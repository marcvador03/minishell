/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 10:53:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_fd(t_pipe *p)
{
	int	err;

	if (p->r_fd[INPUT] > 2)
	{
		p->r_fd[T_INPUT] = dup(STDIN_FILENO);
		if (p->r_fd[T_INPUT] == -1)
			return (flush_errors(p->args[0], -1), -1);
		err = dup2(p->r_fd[INPUT], STDIN_FILENO);
		if (err == -1)
			return (flush_errors(p->args[0], -1), -1);
	}
	if (p->r_fd[OUTPUT] > 2)
	{
		p->r_fd[T_OUTPUT] = dup(STDOUT_FILENO);
		if (p->r_fd[T_OUTPUT] == -1)
			return (flush_errors(p->args[0], -1), -1);
		err = dup2(p->r_fd[OUTPUT], STDOUT_FILENO);
		if (err == -1)
			return (flush_errors(p->args[0], -1), -1);
	}
	return (0);
}

int	close_redir_fd(t_pipe *p)
{
	int	err;

	if (p->r_fd[INPUT] > 2)
	{
		close(p->r_fd[INPUT]);
		err = dup2(p->r_fd[T_INPUT], STDIN_FILENO);
		if (err == -1)
			flush_errors(p->args[0], -1);
		close(p->r_fd[T_INPUT]);
	}
	if (p->r_fd[OUTPUT] > 2)
	{
		close(p->r_fd[OUTPUT]);
		err = dup2(p->r_fd[T_OUTPUT], STDOUT_FILENO);
		if (err == -1)
			flush_errors(p->args[0], -1);
		close(p->r_fd[T_OUTPUT]);
	}
	return (0);
}

int	close_pipes(t_pipe *p)
{
	p = p->head;
	while (p != NULL)
	{
		close(p->fd[READ_END]);
		close(p->fd[WRITE_END]);
		p = p->next;
	}
	return (0);
}
