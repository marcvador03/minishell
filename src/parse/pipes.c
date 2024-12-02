/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:53:58 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 19:50:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_redirs(t_pipe *p);
char	**create_args(t_pipe *p);
int		get_fdin_redir(t_pipe *p);
int		get_fdout_redir(t_pipe *p);

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

static int	fill_in_pipe(t_pipe *p)
{
	p->redirs = create_redirs(p);
	p->args = create_args(p);
	if (p->redirs == NULL || p->args == NULL)
		return (free_pipe(p), -1);
	sh_trim_list_strings(p->args, "\"");
	sh_trim_list_strings(p->redirs, "\"");
	p->r_fd[INPUT] = get_fdin_redir(p);
	p->r_fd[OUTPUT] = get_fdout_redir(p);
	return (0);
}

int	get_next_pipe(t_pipe *p, char **line)
{
	int		i;

	if (*line == NULL)
		return (-1);
	i = 0;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == 34 || (*line)[i] == 39)
			i += sh_jump_to(*line + i, (*line)[i]);
		if ((*line)[i] == '|' || (*line)[i] == '\0')
		{
			p->p_line = sh_strcut(*line, 0, i);
			p->p_line = sh_strtrim(p->p_line, " ", 0);
			*line = sh_strstrip(line, 0, i);
			*line = sh_strtrim(*line, "|", 0);
			fill_in_pipe(p);
			return (0);
		}
		i++;
	}
	p->p_line = sh_strcut(*line, 0, i);
	p->p_line = sh_strtrim(p->p_line, " ", 0);
	*line = sh_strstrip(line, 0, i);
	fill_in_pipe(p);
	return (0);
}
