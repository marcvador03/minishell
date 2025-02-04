/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 11:06:18 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/04 09:52:44 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_separation(char *line, t_parse *q);
int	count_words_dollar(t_pipe *p, t_parse *q);
int	count_words_rd(t_pipe *p, t_parse *q);

int	get_words_loop(t_pipe *p, t_parse *q)
{
	q->prev_pos = q->i;
	if (q->t_line[q->i] == ' ')
	{
		q->i += sh_skip(q->t_line + q->i, ' ');
		if (create_separation(q->t_line, q) == -1)
			return (-1);
		return (get_words_loop(p, q));
	}
	if (count_words_rd(p, q) == 1)
		return (1);
	if (count_words_dollar(p, q) == 1)
		return (1);
	while (q->t_line[q->i] == 34 || q->t_line[q->i] == 39)
	{
		q->i += sh_jump_to(q->t_line + q->i, q->t_line[q->i]);
		if (q->t_line[q->i] == '\0')
			return (1);
		return (get_words_loop(p, q));
	}
	q->flag_sep = 0;
	if (q->t_line[q->i] == '\0')
		return (1);
	q->i++;
	return (0);
}

static int	count_words(t_pipe *p)
{
	t_parse	q;

	if (p->p_line == NULL || p->p_line[0] == '\0')
		return (-1);
	q.t_line = ft_strdup(p->p_line);
	init_parse(&q);
	q.k = 1;
	q.status = 1;
	while (q.t_line[q.i] != '\0')
	{
		if (get_words_loop(p, &q) == 1)
			return (free_s(q.t_line), q.k);
	}
	return (free_s(q.t_line), q.k);
}

static char	**get_sep_words_init(t_pipe *p, int *status, t_parse *q, int *n)
{
	char	**parse;

	if (p->p_line == NULL)
		return (NULL);
	init_parse(q);
	*n = count_words(p);
	if (p->p_line == NULL)
		return (NULL);
	parse = (char **)ft_calloc(sizeof(char *), *n + 1);
	if (parse == NULL)
		return (set(flush_errors("", 202, 0), status), NULL);
	q->t_line = p->p_line;
	return (parse);
}

char	**get_sep_words(t_pipe *p, int *status)
{
	t_parse		q;
	int			x;
	int			n;

	q.parse = get_sep_words_init(p, status, &q, &n);
	if (q.parse == NULL)
		return (NULL);
	while (q.t_line[q.i] != '\0')
	{
		q.prev_pos = q.i;
		x = get_words_loop(p, &q);
		if (x == -1)
		{
			*status = flush_errors("", 202, 0);
			p->p_line = q.t_line;
			return (free_d(q.parse), NULL);
		}
		else if (x == 1)
			break ;
	}
	q.prev_pos = q.beg_sep;
	p->p_line = q.t_line;
	if (create_separation(p->p_line, &q) == -1)
		return (set(flush_errors("", 202, 0), status), free_d(q.parse), NULL);
	return (q.parse);
}
