/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:20:19 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/30 12:15:03 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	init_heredoc(char *line);
int	get_rd_flag(char *rd);

int	get_fdin_redir(t_pipe *p, int n)
{
	int	i;
	int	fd;
	int	rd;

	/*fd = (int *)ft_calloc(sizeof(int), n);
	if (fd == NULL)
		return (-1);*/
	i = 0;
	fd = -2;
	while (p->redirs[i] != NULL)
	{
		if (fd != -2)
			close(fd);
		rd = get_rd_flag(p->rd[i]);
		if (rd == 2)
			fd = open(p->redirs[i], O_RDONLY, 0700);
		else if (rd == 4)
			fd = init_heredoc(p->redirs[i]);
		/*if (p->rd[i] & (1 << 0))
		{
			x = fd;
			if (x == -1)
				return (close(fd), -1);
		}*/
		i++;
	}
	if (fd == -2)
		fd = STDIN_FILENO;
	return (fd);
}

int	get_fdout_redir(t_pipe *p)
{
	int	i;
	int	fd;
	int	rd;

	/*fd = (int *)ft_calloc(sizeof(int), n);
	if (fd == NULL)
		return (-1);*/
	i = 0;
	fd = -2;
	while (p->redirs[i] != NULL)
	{
		if (fd != -2)
			close(fd);
		rd = get_rd_flag(p->rd[i]);
		if (rd == 3)
			fd = open(p->redirs[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else if (rd == 1)
			fd = open(p->redirs[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		/*if (!(p->rd[i] & (1 << 0)))
		{
			x = fd;
			if (x == -1)
				return (close(fd), -1);
		}*/
		i++;
	}
	if (fd == -2)
		fd = STDOUT_FILENO;
	return (fd);
}



