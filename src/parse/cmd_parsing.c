/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:22:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/20 11:00:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parse(t_parse *q);
int		count_quotes(char *line);

static int	create_separation(char *line, t_parse *q)
{
	if (q->i - q->prev_pos >= 1)
	{
		q->seps[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
		q->seps[q->j] = sh_trim_spaces(q->seps[q->j]);
		if (q->seps[q->j] == NULL)
			return (-1);
		q->beg_sep = q->i;
		//q->prev_pos = q->i;
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
	while (none_of_char(line[q->i], " ,>,<") == 1)
	{
		if (line[q->i] == 39 || line[q->i] == 34)
			q->i += sh_jump_to(line + q->i, line[q->i]);
		else
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

static int	create_rd_separation(char *line, t_parse *q)
{
	if (q->i >= 1 && line[q->i - 1] != ' ')
	{
		q->seps[q->j] = ft_substr(line, q->beg_sep, q->i  - q->beg_sep);
		q->seps[q->j] = sh_trim_spaces(q->seps[q->j]);
		if (q->seps[q->j++] == NULL)
			return (-1);
	}
	q->beg_sep = q->i;
	while (line[q->i] == '>' || line[q->i] == '<')
		q->i++;
	q->i += sh_skip(line + q->i, ' ');
	q->seps[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
	q->seps[q->j] = sh_trim_spaces(q->seps[q->j]);
	if (q->seps[q->j++] == NULL)
		return (-1);
	q->beg_sep = q->i;
	q->flag_jump = 1;
	if (line[q->i] == '\0')
		return (1);
	return (0);
}

int	separate_quotes(char *line, t_parse *q)
{
	q->i += sh_jump_to(line + q->i, line[q->i]);
	if (line[q->i] == '\0')
		return (1);
	while (none_of_char(line[q->i], " ,\",',>,<") == 1)
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

static int	get_sep_quotes_loop(char *line, t_parse *q)
{
	q->flag_jump = 0;
	q->prev_pos = q->i;
	if (line[q->i] == ' ')
	{
		q->i += sh_skip(line + q->i, ' ');
		if (create_separation(line, q) == -1)
			return (-1);
		q->prev_pos = q->i;
		q->flag_jump = 1;
	}
	/*if (create_separation(line, q) == -1)
		return (-1);*/
	if (line[q->i] == '\0')
		return (1);
	//q->prev_pos = q->i;
	if (line[q->i] == '<' || line[q->i] == '>')
		if (create_rd_separation(line, q) == 1)
			return (1);
	q->prev_pos = q->i;
	if (line[q->i] == '$')
	{
		if (separate_dollar(line, q) == 1)
			return (1);
		if (create_separation(line, q) == 1)
			return (1);
		q->prev_pos = q->i;
	}
	while (line[q->i] == 34 || line[q->i] == 39)
	{
		if (separate_quotes(line, q) == 1)
			return (1);
	}
	if (create_separation(line, q) == -1)
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

	if (line == NULL)
		return (NULL);
	init_parse(&q);
	n = count_quotes(line);
	q.seps = (char **)ft_calloc(sizeof(char *), n + 1);
	if (q.seps == NULL)
		return (NULL);
	while (line[q.i] != '\0')
	{
		x = get_sep_quotes_loop(line, &q);
		if (x == -1)
			return (flush_errors("", 202), free_d(q.seps), NULL);
		else if (x == 1)
			break ;
	}
	q.prev_pos = q.beg_sep;
	if (create_separation(line, &q) == -1)
		return (flush_errors("", 202), free_d(q.seps), NULL);
	return (q.seps);
}
