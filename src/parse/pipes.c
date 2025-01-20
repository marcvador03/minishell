/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:53:58 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/19 15:15:36 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_redirs(t_pipe *p);
char	**create_args(t_pipe *p);
int		get_fds_redir(t_pipe *p);
int		close_redir_fd_mult(t_pipe *p);
int	create_parsing(t_pipe *p);

int	count_pipes(char *line)
{
	int	n;
	int	i;

	if (line == NULL)
		return (0);
	n = 1;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			break ;
		if (line[i] == '|')
		{
			i += sh_skip(line, ' ');
			n++;
		}
		i++;
	}
	return (n);
}

/*static int	fill_in_pipe(t_pipe *p)
{
	p->redirs = create_redirs(p);
	if (p->redirs == NULL)
		return (2);
	p->args = create_args(p);
	if (p->args == NULL)
		return (flush_errors("", -1), -1);
	if (get_fds_redir(p) == -1)
		return (close_redir_fd_mult(p->head), 2);
	return (0);
}*/

int	get_next_pipe(t_pipe *p, char *t_line)
{
	int		i;

	i = 0;
	while (t_line[i] != '\0')
	{
		while (t_line[i] == 34 || t_line[i] == 39)
			i += sh_jump_to(t_line + i, t_line[i]);
		if (t_line[i] == '|' || t_line[i] == '\0')
		{
			p->p_line = ft_substr(t_line, 0, i);
			if (p->p_line == NULL)
				return (flush_errors("", -1), 2);
			else if (sh_check_empty(p->p_line) != 0)
				return (flush_errors("", 205), 2);
			if (t_line[i] == '|')
				ft_memset(t_line, ' ', i + 1);
			else
				ft_memset(t_line, ' ', i);
			return ((create_parsing(p)));
		}
		i++;
	}
	p->p_line = ft_strdup(t_line);
	return (create_parsing(p));
}
