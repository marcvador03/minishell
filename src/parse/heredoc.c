/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:57:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/16 15:58:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_hd_input(char *eof, int fd)
{
	char	*hd_input;
	int		len;

	init_signal(0, 1);
	while (1)
	{
		hd_input = readline("> ");
		if (hd_input == NULL)
			return (close(fd), -1);
		len = max(ft_strlen(eof), ft_strlen(hd_input));
		if (ft_strncmp(hd_input, eof, len) != 0)
			ft_putendl_fd(hd_input, fd);
		else
			return (close(fd), free(hd_input), 0);
		free(hd_input);
	}
	return (close(fd), 0);
}

static int	close_heredoc(int wstatus, int fd)
{
	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd == -1)
		return (flush_errors("heredoc", -1), -1);
	else if (fd > 0)
		unlink(".heredoc_tmp");
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 130 || WEXITSTATUS(wstatus) == 131)
		{
			rl_on_new_line();
			set_gstatus(WEXITSTATUS(wstatus));
			close(fd);
			return (-1);
		}
	}
	return (fd);
}

int	init_heredoc(char *line)
{
	int	pid;
	int	wstatus;
	int	fd;

	init_signal(1, 1);
	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0600);
	if (fd == -1)
		return (flush_errors("heredoc", -1), -1);
	pid = fork();
	if (pid == -1)
		return (flush_errors("heredoc", -1), -1);
	if (pid == 0)
		exit(get_hd_input(line, fd));
	waitpid(pid, &wstatus, 0);
	close(fd);
	fd = close_heredoc(wstatus, fd);
	return (fd);
}
