/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:20:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 12:57:45 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc(t_redirs *r, char *line, int *r_status);

int	get_rd_flag(char *rd)
{
	if (ft_strncmp(rd, ">", ft_strlen(rd)) == 0)
		return (1);
	else if (ft_strncmp(rd, ">>", ft_strlen(rd)) == 0)
		return (3);
	else if (ft_strncmp(rd, "<", ft_strlen(rd)) == 0)
		return (2);
	else if (ft_strncmp(rd, "<<", ft_strlen(rd)) == 0)
		return (4);
	return (0);
}

static int	get_fdout(t_redirs *r, int rd, int *err, int i)
{
	int	fd;

	fd = -2;
	if (r->fd[OUTPUT] > 2)
		close(r->fd[OUTPUT]);
	if (rd == 3)
		fd = open(r->redirs[i], O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (rd == 1)
		fd = open(r->redirs[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (set(flush_errors(r->redirs[i], -1, 0), err), -1);
	if (fd == -2)
		fd = STDOUT_FILENO;
	return (fd);
}

static int	get_fdin(t_redirs *r, int rd, int *err, int i)
{
	int	fd;

	fd = -2;
	if (r->fd[INPUT] > 2)
		close(r->fd[INPUT]);
	if (rd == 2)
		fd = open(r->redirs[i], O_RDONLY, 0700);
	if (fd == -1)
		return (set(flush_errors(r->redirs[i], -1, 0), err), -1);
	else if (rd == 4)
		fd = init_heredoc(r, r->redirs[i], err);
	if (fd == -1)
		return (-1);
	if (fd == -2)
		fd = STDIN_FILENO;
	return (fd);
}

int	get_fds_redir(t_redirs *r, int *err)
{
	int	fd;
	int	rd;
	int	i;

	fd = -2;
	i = 0;
	if (r == NULL)
		return (0);
	while (r->redirs[i] != NULL)
	{
		rd = get_rd_flag(r->rd[i]);
		if (rd == 2 || rd == 4)
			r->fd[INPUT] = get_fdin(r, rd, err, i);
		else if (rd == 1 || rd == 3)
			r->fd[OUTPUT] = get_fdout(r, rd, err, i);
		if (r->fd[INPUT] == -1 || r->fd[OUTPUT] == -1)
			return (-1);
		i++;
	}
	return (fd);
}

int	create_pipes_child(t_pipe *p)
{
	t_pipe	*o;

	o = p->prev;
	if (p == p->head)
		if (dup2(p->fd[WRITE_END], STDOUT_FILENO) == -1)
			return (close_pipes(p), -1);
	if (p != p->head && p->next != NULL)
	{
		if (dup2(o->fd[READ_END], STDIN_FILENO) == -1)
			return (close_pipes(p), -1);
		if (dup2(p->fd[WRITE_END], STDOUT_FILENO) == -1)
			return (close_pipes(p), -1);
	}
	else if (p != p->head && p->next == NULL)
	{
		if (dup2(o->fd[READ_END], STDIN_FILENO) == -1)
			return (close_pipes(p), -1);
	}
	return (0);
}
