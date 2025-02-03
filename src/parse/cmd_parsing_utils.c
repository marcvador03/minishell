/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 14:41:24 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/03 21:06:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_words_loop(t_pipe *p, t_parse *q);

static int	create_separation_exp(char *line, t_parse *q)
{
	int		i;
	char	*tmp;

	if (q->status == 1)
		return (set(q->k + 1, &q->k), 0);
	i = 0;
	while (line[i] != '\0' && i != q->prev_pos2)
	{
		if (line[i] == 39 || line[i] == 34)
		{
			q->tk = (line[i] ^ 1);
			q->tk = (q->tk ^ (1 << 2));
		}
		i++;
	}
	q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
	q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
	tmp = ft_strjoin(&q->tk, q->parse[q->j]);
	free_s(q->parse[q->j]);
	q->parse[q->j] = ft_strjoin(tmp, &q->tk);
	if (q->parse[q->j] == NULL)
		return (free_s(tmp), -1);
	q->beg_sep = q->i;
	q->j++;
	return (free_s(tmp), 0);
}

int	create_separation(char *line, t_parse *q)
{
	if (q->status == 1)
	{
		q->k = q->k + 1;
		return (0);
	}
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

static int	count_within_dollar(t_parse *q)
{
	if (q->t_line[q->i] == ' ')
		q->i += sh_skip(q->t_line + q->i, ' ');
	while (q->i < q->prev_pos2 && q->t_line[q->i] != '\0')
	{
		if (q->t_line[q->i] == ' ')
		{
			q->i += sh_skip(q->t_line + q->i, ' ');
			if (create_separation_exp(q->t_line, q) == -1)
				return (-1);
			if (q->t_line[q->i] == '\0')
				break ;
			q->beg_sep = q->i;
		}
		else
			q->i++;
	}
	return (0);
}

int	count_words_dollar(t_pipe *p, t_parse *q)
{
	if (q->t_line[q->i] == '$' && q->t_line[q->i + 1] != '\0')
	{
		if (q->flag_sep == 1)
			q->i += sh_jump_to(q->t_line + q->i, ' ');
		else if (ft_isalnum_plus(q->t_line[q->i + 1]) == 1)
		{
			if (q->t_line[q->i + 1] == ' ')
				return (0);
			q->prev_pos2 = q->i;
			q->t_line = expand_variable(p->sh, q->t_line, &q->prev_pos2);
			if (q->t_line == NULL)
				return (1);
			count_within_dollar(q);
			q->flag_sep = 0;
			return(get_words_loop(p, q));
		}
		q->flag_sep = 0;
	}
	return (0);
}

int	count_words_rd(t_pipe *p, t_parse *q)
{
	if (q->t_line[q->i] == '<' || q->t_line[q->i] == '>')
	{
		if (create_separation(q->t_line, q) == -1)
			return (-1);
		q->beg_sep = q->i;
		while (q->t_line[q->i] == '>' || q->t_line[q->i] == '<')
			q->i++;
		if (q->t_line[q->i] == '\0')
			return (1);
		if (q->i >= 2)
		{
			if (q->t_line[q->i - 1] == '<' && q->t_line[q->i - 2] == '<')
				q->flag_sep = 1;
		}
		q->i += sh_skip(q->t_line + q->i, ' ');
		if (create_separation(q->t_line, q) == -1)
			return (-1);
		return(get_words_loop(p, q));
	}
	return (0);
}
