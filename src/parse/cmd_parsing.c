/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:22:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/18 16:54:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parse(t_parse *q);
int		count_quotes(char *line);

static int	create_separation(char *line, t_parse *q, char **seps)
{
	if (q->i - q->prev_pos >= 1)
	{
		*seps = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
		if (*seps == NULL)
			return (-1);
		q->beg_sep = q->i;
		q->j++;
		return (0);
	}
	else
		return (0);
}

int	separate_dollar(char *line, t_parse *q)
{
	q->i++;
	if (line[q->i] == '\0')
		return (1);
	while (line[q->i] != ' ' && line[q->i] != 34 && line[q->i] != 39)
	{
		if (line[q->i + 1] == 39 || line[q->i + 1] == 34)
			q->i += sh_jump_to(line + q->i + 1, line[q->i + 1]);
		q->i++;
		if (line[q->i] == '\0')
			return (1);
	}
	if (line[q->i] == '\0')
		return (1);
	q->i += sh_skip(line + q->i, ' ');
	q->flag_jump = 1;
	return (0);
}

int	separate_quotes(char *line, t_parse *q)
{
	q->i += sh_jump_to(line + q->i, line[q->i]);
	if (line[q->i] == '\0')
		return (1);
	while (line[q->i] != ' ' && line[q->i] != 34 && line[q->i] != 39)
	{
		q->i++;
		if (line[q->i] == '\0')
			return (1);
	}
	if (line[q->i] == ' ')
	{
		q->i += sh_skip(line + q->i, ' ');
		q->flag_jump = 1;
	}
	return (0);
}

static int	get_sep_quotes_loop(char *line, t_parse *q, char ***seps)
{
	q->flag_jump = 0;
	q->prev_pos = q->i;
	if (line[q->i] == ' ' || line[q->i] == '$')
	{
		q->i += sh_skip(line + q->i, ' ');
		q->flag_jump = 1;
	}
	if (create_separation(line, q, seps[q->j]) == -1)
		return (-1);
	q->prev_pos = q->i;
	if (line[q->i] == '$')
		if (separate_dollar(line, q) == 1)
			return (1);
	while (line[q->i] == 34 || line[q->i] == 39)
		if (separate_quotes(line, q) == 1)
			return (1);
	if (create_separation(line, q, seps[q->j]) == -1)
		return (-1);
	if (line[q->i] == '\0')
		return (1);
	if (q->flag_jump == 0)
		q->i++;
	return (0);
}

char	**get_sep_quotes(char *line)
{
	t_parse		q;
	int			x;
	int			n;
	char		**seps;

	if (line == NULL)
		return (NULL);
	init_parse(&q);
	n = count_quotes(line);
	seps = (char **)ft_calloc(sizeof(char *), n + 1);
	if (seps == NULL)
		return (NULL);
	while (line[q.i] != '\0')
	{
		x = get_sep_quotes_loop(line, &q, &seps);
		if (x == -1)
			return (flush_errors("", 202), free_d(seps), NULL);
		else if (x == 1)
			break ;
	}
	q.prev_pos = q.beg_sep;
	if (create_separation(line, &q, &seps[q.j]) == -1)
		return (flush_errors("", 202), free_d(seps), NULL);
	return (seps);
}
