/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:34:46 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 22:12:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	count_quotes_rd_dollar(char *line, t_parse *q, int *n)
{
	if (line[q->i] == '<' || line[q->i] == '>')
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
	}
	if (line[q->i] == '$')
	{
		if (separate_dollar(line, q) == 1)
			return (1);
		if (q->i - q->prev_pos >= 1)
		{
			q->prev_pos = q->i;
			*n = *n + 1;
		}
	}
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
	if (count_quotes_rd_dollar(line, q, n) == 1)
		return (1);
	while (line[q->i] == 34 || line[q->i] == 39)
		if (separate_quotes(line, q) == 1)
			return (1);
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
