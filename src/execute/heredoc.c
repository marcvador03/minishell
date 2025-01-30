/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:57:19 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/31 00:07:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_dollar(t_redirs *r, char *line)
{
	int		i;
	int		flag_jump;

	i = 0;
	while (line[i] != '\0')
	{
		flag_jump = 0;
		if (line[i] == '$' && ft_isalnum_plus_q(line[i + 1]) == 1)
		{
			line = expand_variable(r->sh, line, &i);
			flag_jump = 1;
			if (line[i] == '\0')
				return (line);
		}
		if (flag_jump == 0)
			i++;
	}
	return (line);
}

static int	close_other_fds(t_redirs *r, int *err)
{
	if (r->sh->up != NULL && r->sh->up->r != NULL)
	{
		close_redir_fd_single(r->sh->up->r, err, "");
		close_redir_fd_sh(r->sh->up);
	}
	return (*err);
}

static int	get_hd_input(t_redirs *r, char *eof, int fd)
{
	char	*hd_input;
	int		len;
	int		err;

	init_signal(0, 1);
	close_other_fds(r, &err);
	while (1)
	{
		hd_input = readline("> ");
		if (hd_input == NULL)
			return (close(fd), -1);
		len = max(ft_strlen(eof), ft_strlen(hd_input));
		if (ft_strncmp(hd_input, eof, len) != 0)
		{
			if (r->hd_flag == 0)
				hd_input = trim_dollar(r, hd_input);
			ft_putendl_fd(hd_input, fd);
		}
		else
			return (close(fd), free(hd_input), -1);
		free(hd_input);
	}
	return (close(fd), 0);
}

static int	close_heredoc(int wstatus, int fd, int *r_status)
{
	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd == -1)
		return (set(flush_errors("heredoc", -1, 0), r_status), -1);
	else if (fd > 0)
		unlink(".heredoc_tmp");
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 130 || WEXITSTATUS(wstatus) == 131)
		{
			rl_on_new_line();
			*r_status = WEXITSTATUS(wstatus);
			close(fd);
			return (-1);
		}
	}
	return (fd);
}

int	init_heredoc(t_redirs *r, char *line, int *r_status)
{
	int	pid;
	int	wstatus;
	int	fd;

	init_signal(1, 1);
	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0600);
	if (fd == -1)
		return (flush_errors("heredoc", -1, 0), -1);
	pid = fork();
	if (pid == -1)
		return (flush_errors("heredoc", -1, 0), -1);
	if (pid == 0)
		exit(get_hd_input(r, line, fd));
	waitpid(pid, &wstatus, 0);
	close(fd);
	fd = close_heredoc(wstatus, fd, r_status);
	return (fd);
}
