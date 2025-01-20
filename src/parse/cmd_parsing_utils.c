/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:34:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/20 01:03:31 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		separate_dollar(char *line, t_parse *q);
int		separate_quotes(char *line, t_parse *q);

void	init_parse(t_parse *q)
{
	q->i = 0;
	q->j = 0;
	q->prev_pos = 0;
	q->beg_sep = 0;
	q->len = 0;
	q->flag_jump = 0;
}

static int	separate_rd(char *line, t_parse *q, int *n)
{
	if (q->i >= 1 && line[q->i - 1] != ' ')
		*n = *n + 1;
	while (line[q->i] == '>' || line[q->i] == '<')
		q->i++;
	if (line[q->i] == '\0')
		return (1);
	q->i += sh_skip(line + q->i, ' ');
	*n = *n + 1;
	q->flag_jump = 1;
	return (0);
}

static int	count_quotes_loop(char *line, t_parse *q, int *n)
{
	q->flag_jump = 0;
	q->prev_pos = q->i;
	if (line[q->i] == ' ')
	{
		q->i += sh_skip(line + q->i, ' ');
		*n = *n + 1;
		q->prev_pos = q->i;
		q->flag_jump = 1;
	}
	/*if (q->i - q->prev_pos >= 1)
		*n = *n + 1;
	q->prev_pos = q->i;*/
	if (line[q->i] == '<' || line[q->i] == '>')
		if (separate_rd(line, q, n) == 1)
			return (1);
	if (line[q->i] == '$')
	{
		if (separate_dollar(line, q) == 1)
			return (1);
		if (q->i - q->prev_pos >= 1)
			*n = *n + 1;
	}
	while (line[q->i] == 34 || line[q->i] == 39)
	{
		if (q->i - q->prev_pos >= 1)
			*n = *n + 1;
		if (separate_quotes(line, q) == 1)
			return (1);
	}
	if (q->i - q->prev_pos >= 1)
		*n = *n + 1;
	if (line[q->i] == '\0')
		return (1);
	if (q->flag_jump == 0)
		q->i++;
	return (0);
}

int	count_quotes(char *line)
{
	int		n;
	t_parse	q;

	if (line == NULL || line[0] == '\0')
		return (-1);
	n = 1;
	init_parse(&q);
	while (line[q.i] != '\0')
	{
		if (count_quotes_loop(line, &q, &n) == 1)
			return (n);
	}
	return (n);
}
