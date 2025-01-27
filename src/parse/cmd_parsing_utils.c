/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:34:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/27 18:56:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*trim_dollar(t_redirs *r, char *line, t_parse *q, int f_exp);

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
		q->i += sh_skip(line + q->i, ' ') - 1;
		q->flag_jump = 0;
	}
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

static int	count_quotes_rd_dollar(t_pipe *p, t_parse *q, int *n)
{
	char	*t_line;

	t_line = p->p_line;
	if (t_line[q->i] == '<' || t_line[q->i] == '>')
	{
		if (q->i >= 1 && t_line[q->i - 1] != ' ')
			*n = *n + 1;
		while (t_line[q->i] == '>' || t_line[q->i] == '<')
			q->i++;
		if (t_line[q->i] == '\0')
			return (1);
		q->i += sh_skip(t_line + q->i, ' ');
		*n = *n + 1;
		q->flag_jump = 1;
	}
	if (t_line[q->i] == '$')
	{
		q->prev_pos = q->i;
		t_line = expand_variable(p->sh, t_line, &q->i);
		p->p_line = t_line;
		q->i = q->prev_pos;
		/*if (separate_dollar(line, q) == 1)
			return (1);
		if (q->i - q->prev_pos >= 1)
		{
			q->prev_pos = q->i;
			*n = *n + 1;
		}*/
	}
	return (0);
}

static int	count_quotes_loop(t_pipe *p, t_parse *q, int *n)
{
	q->flag_jump = 0;
	q->prev_pos = q->i;
	if (p->p_line[q->i] == ' ')
	{
		q->i += sh_skip(p->p_line + q->i, ' ');
		*n = *n + 1;
		q->prev_pos = q->i;
		q->flag_jump = 1;
	}
	if (count_quotes_rd_dollar(p, q, n) == 1)
		return (1);
	while (p->p_line[q->i] == 34 || p->p_line[q->i] == 39)
		if (separate_quotes(p->p_line, q) == 1)
			return (1);
	if (q->i - q->prev_pos >= 1)
		*n = *n + 1;
	if (p->p_line[q->i] == '\0')
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
	n = 1;
	init_parse(&q);
	while (p->p_line[q.i] != '\0')
	{
		if (count_quotes_loop(p, &q, &n) == 1)
			return (n);
	}
	return (n);
}
