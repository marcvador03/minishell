/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:20:19 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/03 01:05:34 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_heredoc(char *line);

static int	get_rd_flag(char *rd)
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

int	get_fdin_redir(t_pipe *p)
{
	int	i;
	int	fd;
	int	rd;

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
		i++;
	}
	if (fd == -2)
		fd = STDOUT_FILENO;
	return (fd);
}
