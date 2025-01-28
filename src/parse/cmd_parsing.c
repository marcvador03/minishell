/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 15:22:36 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 15:43:19 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_quotes(t_pipe *p);

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

static char	**get_sep_quotes_init(t_pipe *p, int *status, t_parse *q, int *n)
{
	char	**parse;

	if (p->p_line == NULL)
		return (NULL);
	init_parse(q);
	*n = count_quotes(p);
	if (p->p_line == NULL)
		return (NULL);
	parse = (char **)ft_calloc(sizeof(char *), *n + 1);
	if (parse == NULL)
		return (set(flush_errors("", 202, 0), status), NULL);
	return (parse);
}

char	**get_sep_quotes(t_pipe *p, int *status)
{
	t_parse		q;
	int			x;
	int			n;

	q.parse = get_sep_quotes_init(p, status, &q, &n);
	if (q.parse == NULL)
		return (NULL);
	while (p->p_line[q.i] != '\0')
	{
		q.flag_jump = 0;
		q.prev_pos = q.i;
		x = get_sep_quotes_loop(p->p_line, &q);
		if (x == -1)
		{
			*status = flush_errors("", 202, 0);
			return (free_d(q.parse), NULL);
		}
		else if (x == 1)
			break ;
	}
	q.prev_pos = q.beg_sep;
	if (create_separation(p->p_line, &q) == -1)
		return (set(flush_errors("", 202, 0), status), free_d(q.parse), NULL);
	return (q.parse);
}
