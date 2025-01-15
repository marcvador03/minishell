/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:20:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/15 23:04:35 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc(char *line);

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

static int	get_fdout(char *r_path, int rd, int pfd)
{
	int	fd;

	fd = -2;
	if (pfd > 2)
		close(pfd);
	if (rd == 3)
		fd = open(r_path, O_CREAT | O_RDWR | O_APPEND, 0644);
	else if (rd == 1)
		fd = open(r_path, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		return (flush_errors(r_path, -1), -1);
	if (fd == -2)
		fd = STDOUT_FILENO;
	return (fd);
}

static int	get_fdin(char *r_path, int rd, int pfd)
{
	int	fd;

	fd = -2;
	if (pfd > 2)
		close(pfd);
	if (rd == 2)
		fd = open(r_path, O_RDONLY, 0700);
	if (fd == -1)
		return (flush_errors(r_path, -1), -1);
	else if (rd == 4)
		fd = init_heredoc(r_path);
	if (fd == -1)
		return (-1);
	if (fd == -2)
		fd = STDIN_FILENO;
	return (fd);
}

int	get_fds_redir(t_pipe *p)
{
	int	i;
	int	fd;
	int	rd;
	
	i = 0;
	fd = -2;
	while (p->redirs[i] != NULL)
	{
		rd = get_rd_flag(p->rd[i]);
		if (rd == 2 || rd == 4)
			p->r_fd[INPUT] = get_fdin(p->redirs[i], rd, p->r_fd[INPUT]);
		else if (rd == 1 || rd == 3)
			p->r_fd[OUTPUT] = get_fdout(p->redirs[i], rd, p->r_fd[OUTPUT]);
		if (p->r_fd[INPUT] == -1 || p->r_fd[OUTPUT] == -1)
			return (-1);
		i++;
	}
	return (fd);
}

/*int	get_fdin_redir(t_pipe *p)
{
	int	i;
	int	fd;
	int	rd;

	i = 0;
	fd = -2;
	while (p->redirs[i] != NULL)
	{
		rd = get_rd_flag(p->rd[i]);
		if (fd != -2 && (rd == 2 || rd == 4))
			close(fd);
		if (rd == 2)
			fd = open(p->redirs[i], O_RDONLY, 0700);
		if (fd == -1)
			return (flush_errors(p->redirs[i], -1), 0);
		else if (rd == 4)
			fd = init_heredoc(p->redirs[i]);
		if (fd == -1)
			return (-1);
		i++;
	}
	if (fd == -2)
		fd = STDIN_FILENO;
	return (fd);
}*/

/*int	get_fdout_redir(t_pipe *p)
{
	int	i;
	int	fd;
	int	rd;

	i = 0;
	fd = -2;
	while (p->redirs[i] != NULL)
	{
		rd = get_rd_flag(p->rd[i]);
		if (fd != -2 && (rd == 1 || rd == 3))
			close(fd);
		if (rd == 3)
			fd = open(p->redirs[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else if (rd == 1)
			fd = open(p->redirs[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
			return (flush_errors(p->redirs[i], -1), -1);
		i++;
	}
	if (fd == -2)
		fd = STDOUT_FILENO;
	return (fd);
}*/

char	find_next_token(char *line)
{
	int	i;

	if (line == NULL)
		return ('\0');
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == ' ' || line[i] == '<' || line[i] == '>')
			return (line[i]);
		i++;
	}
	return (line[i]);
}
