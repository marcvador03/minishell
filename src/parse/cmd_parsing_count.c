/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_count.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:34:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/01 22:33:48 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_spaces(char *line, int *pos, int *n, t_parse *q);

static int	count_quotes_rd(t_parse *q, int *n)
{
	if (q->t_line[q->i] == '<' || q->t_line[q->i] == '>')
	{
		if (q->i >= 1 && q->t_line[q->i - 1] != ' ')
			*n = *n + 1;
		while (q->t_line[q->i] == '>' || q->t_line[q->i] == '<')
			q->i++;
		if (q->t_line[q->i] == '\0')
			return (1);
		if (q->i >= 2 && q->t_line[q->i - 1] == '<' && q->t_line[q->i - 2] == '<')
			q->flag_sep = 1;
		q->i += sh_skip(q->t_line + q->i, ' ');
		*n = *n + 1;
		q->flag_jump = 1;
	}
	return (0);
}

static int	count_quotes_dollar(t_pipe *p, t_parse *q, int *n)
{
	if (q->t_line[q->i] == '$' && q->t_line[q->i + 1] != '\0')
	{
		if (q->flag_sep == 1)
			q->i += sh_jump_to(q->t_line + q->i, ' ');
		else if (ft_isalnum_plus(q->t_line[q->i + 1]) == 1)
		{
			if (q->t_line[q->i + 1] == ' ')
				return (0);
			q->prev_pos = q->i;
			q->t_line = expand_variable(p->sh, q->t_line, &q->i);
			if (q->t_line == NULL)
				return (1);
			while (q->prev_pos != q->i)
			{
				q->flag_jump = 0;
				if (q->t_line[q->prev_pos] == ' ')
					count_spaces(q->t_line, &q->prev_pos, n, q);
				if (q->flag_jump == 0)
					q->prev_pos++;
			}
			q->flag_jump = 1;
		}
		q->flag_sep = 0;
	}
	return (0);
}

static int	count_quotes_loop(t_pipe *p, t_parse *q, int *n)
{
	q->flag_jump = 0;
	q->prev_pos = q->i;
	if (q->t_line[q->i] == ' ')
	{
		count_spaces(q->t_line, &q->i, n, q);
		q->prev_pos = q->i;
	}
	if (count_quotes_rd(q, n) == 1)
		return (1);
	if (count_quotes_dollar(p, q, n) == 1)
		return (1);
	while (q->t_line[q->i] == 34 || q->t_line[q->i] == 39)
		if (separate_quotes(q->t_line, q) == 1)
			return (1);
	/*if (count_quotes_dollar(p, q, n, t_line) == 1)
		return (1);*/
	q->flag_sep = 0;
	if (q->i - q->prev_pos >= 1)
		*n = *n + 1;
	if (q->t_line[q->i] == '\0')
		return (1);
	if (q->flag_jump == 0)
		q->i++;
	return (0);
}

int	count_quotes(t_pipe *p)
{
	int		n;
	t_parse	q;

	if (p->p_line == NULL || p->p_line[0] == '\0')
		return (-1);
	q.t_line = ft_strdup(p->p_line);
	n = 1;
	init_parse(&q);
	while (q.t_line[q.i] != '\0')
	{
		if (count_quotes_loop(p, &q, &n) == 1)
			return (free_s(q.t_line), n);
	}
	return (free_s(q.t_line), n);
}
