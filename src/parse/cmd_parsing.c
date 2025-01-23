/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:22:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 23:11:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quotes(char *line);
int	separate_quotes(char *line, t_parse *q);
int	separate_dollar(char *line, t_parse *q);

static int	create_separation(char *line, t_parse *q)
{
	if (q->i - q->prev_pos >= 1)
	{
		q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
		q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
		if (q->parse[q->j] == NULL)
			return (-1);
		q->beg_sep = q->i;
		q->j++;
		return (0);
	}
	else
		return (0);
}

static int	create_rd_separation(char *line, t_parse *q)
{
	if (q->i >= 1 && line[q->i - 1] != ' ')
	{
		q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
		q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
		if (q->parse[q->j++] == NULL)
			return (-1);
	}
	q->beg_sep = q->i;
	while (line[q->i] == '>' || line[q->i] == '<')
		q->i++;
	q->i += sh_skip(line + q->i, ' ');
	q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
	q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
	if (q->parse[q->j++] == NULL)
		return (-1);
	q->beg_sep = q->i;
	q->flag_jump = 1;
	if (line[q->i] == '\0')
		return (1);
	return (0);
}

static int	get_sep_quotes_rd_dollar(char *line, t_parse *q)
{
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
	return (0);
}

static int	get_sep_quotes_loop(char *line, t_parse *q)
{
	if (line[q->i] == ' ')
	{
		q->i += sh_skip(line + q->i, ' ');
		if (create_separation(line, q) == -1)
			return (-1);
		q->prev_pos = q->i;
		q->flag_jump = 1;
	}
	if (line[q->i] == '\0')
		return (1);
	if (get_sep_quotes_rd_dollar(line, q) == 1)
		return (1);
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
	q.parse = (char **)ft_calloc(sizeof(char *), n + 1);
	if (q.parse == NULL)
		return (NULL);
	while (line[q.i] != '\0')
	{
		q.flag_jump = 0;
		q.prev_pos = q.i;
		x = get_sep_quotes_loop(line, &q);
		if (x == -1)
			return (flush_errors("", 202, 0), free_d(q.parse), NULL);
		else if (x == 1)
			break ;
	}
	q.prev_pos = q.beg_sep;
	if (create_separation(line, &q) == -1)
		return (flush_errors("", 202, 0), free_d(q.parse), NULL);
	return (q.parse);
}
