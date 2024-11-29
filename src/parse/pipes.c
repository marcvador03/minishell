/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:53:58 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 12:59:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	skip_quotes(char *str);
int	skip_spaces(char *str);

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
			break;
		if (line[i] == '|')
		{
			i += sh_skip(line, ' ');
			n++;
		}
		i++;
	}
	return (n);
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
			p->p_line = sh_strtrim(&p->p_line, " ", 0);
			*line = sh_strstrip(line, 0, i);
			*line = sh_strtrim(line, "|", 0);
			return (0);
		}
		i++;
	}
	p->p_line = sh_strcut(*line, 0, i);
	p->p_line = sh_strtrim(&p->p_line, " ", 0);
	*line = sh_strstrip(line, 0, i);
	return (0);
}


