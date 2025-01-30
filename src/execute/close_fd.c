/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/30 18:59:11 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_fd(t_redirs *r, int *err, char *cmd)
{
	if (r->fd[INPUT] > 2)
	{
		r->fd[T_INPUT] = dup(STDIN_FILENO);
		if (r->fd[T_INPUT] == -1)
			return (set(flush_errors(cmd, -1, 0), err), -1);
		if (dup2(r->fd[INPUT], STDIN_FILENO) == -1)
			return (set(flush_errors(cmd, -1, 0), err), -1);
	}
	if (r->fd[OUTPUT] > 2)
	{
		r->fd[T_OUTPUT] = dup(STDOUT_FILENO);
		if (r->fd[T_OUTPUT] == -1)
			return (set(flush_errors(cmd, -1, 0), err), -1);
		if (dup2(r->fd[OUTPUT], STDOUT_FILENO) == -1)
			return (set(flush_errors(cmd, -1, 0), err), -1);
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

int	close_redir_fd_single(t_redirs *r, int *err, char *cmd)
{
	if (r->fd[INPUT] > 2)
	{
		if (dup2(r->fd[T_INPUT], STDIN_FILENO) == -1)
			*err = flush_errors(cmd, -1, 0);
		close(r->fd[T_INPUT]);
	}
	if (r->fd[OUTPUT] > 2)
	{
		if (dup2(r->fd[T_OUTPUT], STDOUT_FILENO) == -1)
			*err = flush_errors(cmd, -1, 0);
		close(r->fd[T_OUTPUT]);
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
