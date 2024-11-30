/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:57:19 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 19:09:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_hd_input(char *eof, int fd)
{
	char	*hd_input;

	while (1)
	{
		hd_input = readline("> ");
		if (hd_input == NULL)
			return (-1);
		if (ft_strncmp(hd_input, eof, ft_strlen(eof)) != 0)
		{
			ft_putendl_fd(hd_input, fd);
		}
		else
			return (free(hd_input), 0);
		free(hd_input);
	}
	return (0);
}

char	*get_heredoc_eof(char **line, int pos1)
{
	char *t_line;
	int	pos2;
	int	len;

	len = ft_strlen(*line);
	t_line = ft_substr(*line, pos1, len);
	t_line = sh_strtrim(t_line, " ", 0); 
	pos2 = sh_strpos(t_line, " ");
	len = ft_strlen(t_line);
	t_line = sh_strcut2(&t_line, 0, pos2);
	return (t_line);
}

int	init_heredoc(char *line)
{
		int	pid;
		int	wstatus;
		int	fd;

		//pos = sh_strpos(*line, t_rd) + ft_strlen(t_rd);
		//eof = get_heredoc_eof(line, pos);		
		//if (eof == NULL)
		//	return (-1);
		fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd == -1)
			return (-1);
		pid = fork();
		if (pid == -1)
			return (-1);
		if (pid == 0)
			get_hd_input(line, fd);
		waitpid(pid, &wstatus, 0);
		close(fd);
		fd = open(".heredoc_tmp", O_RDONLY);
		if (fd > 0)
			unlink(".heredoc_tmp");
		//manage exit status with WIFEXIT etc
		return (fd);
}
