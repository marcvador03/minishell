/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:20:19 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/24 15:06:19 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	init_heredoc(char *line);

int	get_fdin_redir(t_pipe *p, int n)
{
	int	i;
	int	*fd;
	int	x;

	fd = (int *)ft_calloc(sizeof(int), n);
	if (fd == NULL)
		return (-1);
	i = 0;
	x = STDIN_FILENO;
	while (p->redirs[n][i] != NULL)
	{
		if (p->rd[n][i] == 1)
			fd[i] = open(p->redirs[n][i], O_RDONLY, 0700);
		if (fd[i] == -1)
			return (free_s(fd), -1);
		if (p->rd[n][i] == 3)
			fd[i] = init_heredoc(p->redirs[n][i]);
		if (p->rd[n][i] & (1 << 0))
		{
			x = fd[i];
			if (x == -1)
				return (free_s(fd), -1);
		}
		i++;
	}
	return (free_s(fd), x);
}

int	get_fdout_redir(t_pipe *p, int n)
{
	int	i;
	int	*fd;
	int	x;

	fd = (int *)ft_calloc(sizeof(int), n);
	if (fd == NULL)
		return (-1);
	i = 0;
	x = STDOUT_FILENO;
	while (p->redirs[n][i] != NULL)
	{
		if (p->rd[n][i] == 2)
			fd[i] = open(p->redirs[n][i], O_CREAT | O_RDWR | O_APPEND, 0644);
		else if (p->rd[n][i] == 0)
			fd[i] = open(p->redirs[n][i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd[i] == -1)
			return (free_s(fd), -1);
		if (!(p->rd[n][i] & (1 << 0)))
		{
			x = fd[i];
			if (x == -1)
				return (free_s(fd), -1);
		}
		i++;
	}
	return (free_s(fd), x);
}



