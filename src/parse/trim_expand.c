/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:57:43 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/31 11:41:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	quit_spaces(t_parse *q, char *line, char *f_line)
{
	int	i;

	i = 0;
	q->prev_pos = q->i;
	i = sh_skip(line, line[0]);
	ft_strlcpy(line + 1, line + i, q->len);
	if (line[i] != '\0')
		f_line[q->len - i + q->prev_pos + 1] = '\0';
	else
		f_line[q->len - i] = '\0';
	q->len = ft_strlen(f_line);
	q->i = max(0, q->prev_pos);
	return ;
}

static char	*trim_within_quotes(t_redirs *r, char *line, t_parse *q, int f_exp)
{
	q->prev_pos = q->i;
	q->i += sh_jump_to(line + q->i, line[q->i]);
	if (line[q->prev_pos] == 39 || f_exp == 4)
		return (line);
	while (q->prev_pos != q->i)
	{
		q->flag_jump = 0;
		if (line[q->prev_pos] == '$' && q->prev_pos + 1 != q->i)
		{
			if (ft_isalnum_plus_q(line[q->prev_pos + 1]) == 1)
			{
				line = expand_variable(r->sh, line, &q->prev_pos);
				if (line == NULL)
					return (NULL);
				q->i = max(0, q->i + (ft_strlen(line) - q->len));
				q->len = ft_strlen(line);
				q->flag_jump = 1;
			}
		}
		if (q->flag_jump == 0)
			q->prev_pos++;
	}
	return (line);
}

static char	*trim_quotes(t_redirs *r, char *line, t_parse *q, int f_exp)
{
	while (line[q->i] == 34 || line[q->i] == 39)
	{
		q->len = ft_strlen(line);
		r->hd_flag = 1;
		q->prev_pos2 = q->i;
		line = trim_within_quotes(r, line, q, f_exp);
		if (line == NULL)
			return (NULL);
		ft_strlcpy(line + q->i - 1, line + q->i, q->len);
		q->i = max(0, q->i - 1);
		q->len = ft_strlen(line);
		ft_strlcpy(line + q->prev_pos2, line + q->prev_pos2 + 1, q->len);
		q->i = max(0, q->i - 1);
		q->len = ft_strlen(line);
		q->flag_jump = 1;
		if (line[q->i] == '\0')
			return (line);
		if (line[q->i] == ' ')
			q->flag_jump = 0;
	}
	return (line);
}

static char	*trim_dollar(t_redirs *r, char *line, t_parse *q, int f_exp)
{
	if (ft_isalnum_plus(line[q->i + 1]) == 1 && f_exp != 4)
	{
		line = expand_variable(r->sh, line, &q->i);
		q->len = ft_strlen(line);
		q->flag_jump = 1;
		return (line);
	}
	else if ((line[q->i + 1] == 34 || line[q->i + 1] == 39) && f_exp != 4)
	{
		ft_strlcpy(line + q->i, line + q->i + 1, q->len);
		q->len = ft_strlen(line);
	}
	return (line);
}

char	*trim_expand(t_redirs *r, char *line, int f_exp)
{
	t_parse	q;

	init_parse(&q);
	q.len = ft_strlen(line);
	while (line[q.i] != '\0')
	{
		q.flag_jump = 0;
		if (line[q.i] == ' ' && f_exp != -1)
			quit_spaces(&q, line + q.i, line);
		q.len = ft_strlen(line);
		if (line[q.i] == '$' && line[q.i + 1] != '\0' && f_exp == -1)
			line = trim_dollar(r, line, &q, f_exp);
		if (line == NULL)
			return (NULL);
		if (line[q.i] == '\0')
			return (line);
		line = trim_quotes(r, line, &q, f_exp);
		if (line == NULL)
			return (NULL);
		else if (line[q.i] == '\0')
			return (line);
		if (q.flag_jump == 0)
			q.i++;
	}
	return (line);
}
