/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:57:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/06 18:32:31 by mfleury          ###   ########.fr       */
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
			return (-1);
		len = max(ft_strlen(eof), ft_strlen(hd_input));
		if (ft_strncmp(hd_input, eof, len) != 0)
			ft_putendl_fd(hd_input, fd);
		else
			return (free(hd_input), 0);
		free(hd_input);
	}
	return (0);
}

int	init_heredoc(char *line)
{
	int	pid;
	int	wstatus;
	int	fd;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		exit(get_hd_input(line, fd));
	waitpid(pid, &wstatus, 0);
	close(fd);
	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd > 0)
		unlink(".heredoc_tmp");
	return (fd);
}
