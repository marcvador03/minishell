/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:53:58 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/28 16:11:02 by mfleury          ###   ########.fr       */
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

char	*get_pipes(char **line)
{
	int		i;
	int		pos;
	char	*res;

	if (line == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	res = NULL;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == 34 || (*line)[i] == 39)
			pos = sh_jump_to(*line + i, (*line)[i]);
		else if ((*line)[i] == '|')
			pos = i + sh_skip(*line + i, ' ');
		if (pos > 0)
		{
			res = sh_strcut2(line, 0, pos);
			return (res);
		}
		i++;
	}
	return (*line);
}

char	**identify_pipes(char *s_line, t_pipe **p)
{
	char	**pipes;
	int		cnt_pipes;
	int		i;

	cnt_pipes = count_pipes(s_line);
	pipes = (char **)ft_calloc(sizeof(char *), cnt_pipes + 1);
	if (pipes == NULL)
		return (NULL);
	i = 0;
	while (i < cnt_pipes)
	{
		s_line = sh_strtrim(&s_line, " ", 0);
		pipes[i] = get_pipes(&s_line);
		pipes[i] = sh_strtrim(&pipes[i], " ", 0);
		i++;
	}
	pipes[i] = NULL;
	(*p)->count = cnt_pipes;
	return (set_flag(*p, 0), pipes);
}

