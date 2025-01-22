/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exec_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/22 15:12:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_fd(t_pipe *p)
{
	int	*err;

	err = &p->p_status;
	if (p->r->fd[INPUT] > 2)
	{
		p->r->fd[T_INPUT] = dup(STDIN_FILENO);
		if (p->r->fd[T_INPUT] == -1)
			return (set_status(flush_errors(p->args[0], -1, ""), err), -1);
		*err = dup2(p->r->fd[INPUT], STDIN_FILENO);
		if (*err == -1)
			return (set_status(flush_errors(p->args[0], -1, ""), err), -1);
	}
	if (p->r->fd[OUTPUT] > 2)
	{
		p->r->fd[T_OUTPUT] = dup(STDOUT_FILENO);
		if (p->r->fd[T_OUTPUT] == -1)
			return (set_status(flush_errors(p->args[0], -1, ""), err), -1);
		*err = dup2(p->r->fd[OUTPUT], STDOUT_FILENO);
		if (*err == -1)
			return (set_status(flush_errors(p->args[0], -1, ""), err), -1);
	}
	return (0);
}

int	close_redir_fd_sh(t_shell *sh)
{
	while (sh != NULL)
	{
		if (sh->r->fd[INPUT] > 2)
			close(sh->r->fd[INPUT]);
		if (sh->r->fd[OUTPUT] > 2)
			close(sh->r->fd[OUTPUT]);
		sh = sh->next;
	}
	return (0);
}

int	close_redir_fd_pipe(t_pipe *p)
{
	p = p->head;
	while (p != NULL)
	{
		if (p->r->fd[INPUT] > 2)
			close(p->r->fd[INPUT]);
		if (p->r->fd[OUTPUT] > 2)
			close(p->r->fd[OUTPUT]);
		p = p->next;
	}
	return (0);
}

int	close_redir_fd_single(t_pipe *p)
{
	int	*err;

	err = &p->p_status;
	if (p->r->fd[INPUT] > 2)
	{
		close(p->r->fd[INPUT]);
		*err = dup2(p->r->fd[T_INPUT], STDIN_FILENO);
		if (*err == -1)
			*err = flush_errors(p->args[0], -1, "");
		close(p->r->fd[T_INPUT]);
	}
	if (p->r->fd[OUTPUT] > 2)
	{
		close(p->r->fd[OUTPUT]);
		*err = dup2(p->r->fd[T_OUTPUT], STDOUT_FILENO);
		if (*err == -1)
			*err = flush_errors(p->args[0], -1, "");
		close(p->r->fd[T_OUTPUT]);
	}
	close_redir_fd_pipe(p);
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
