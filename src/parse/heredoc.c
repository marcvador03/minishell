/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:57:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/11 12:59:59 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_hd_input(char *eof, int fd)
{
	char	*hd_input;
	int		len;
	char	*eofn;

	init_signal(0, 1);
	eofn = ft_strjoin(eof, "\n");
	while (1)
	{
		/*write(1, "> ", 2);
		hd_input = get_next_line(STDIN_FILENO);*/
		hd_input = readline("> ");
		if (hd_input == NULL)
			return (free_s(eofn), -1);
		len = max(ft_strlen(eof), ft_strlen(hd_input));
		if (ft_strncmp(hd_input, eofn, len) != 0)
			ft_putstr_fd(hd_input, fd);
		else
			return (free(hd_input), free_s(eofn), 0);
		free(hd_input);
	}
	return (free_s(eofn), 0);
}

int	init_heredoc(char *line)
{
	int	pid;
	int	wstatus;
	int	fd;

	init_signal(1, 1);
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
	if (WIFEXITED(wstatus))
		if (WEXITSTATUS(wstatus) == 130 || WEXITSTATUS(wstatus) == 131)
		{	
			rl_on_new_line();
			return (set_gstatus(WEXITSTATUS(wstatus)), close(fd), -1);
		}
	return (fd);
}
