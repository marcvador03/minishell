/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:43:32 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 22:24:29 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_subshell(t_shell *sh, char *s_line, t_parse *q);

static int	search_next_token(t_shell *sh, char *line, int i)
{
	while (one_of_char(line[i], "&,|") != TRUE && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (i);
	if (line[i] == '&' && one_of_char(line[i + 1], "|,(,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if (line[i] == '&' && line[i + 1] == '&')
		return (i);
	if (line[i] == '|' && one_of_char(line[i + 1], "&,(,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if (line[i] == '|' && line[i + 1] == '|')
		return (i);
	return (search_next_token(sh, line, ++i));
}

static int	set_priority(t_shell *sh, char *s_line, t_parse *q)
{
	q->i += sh_skip(s_line + q->i, ' ');
	if (s_line[q->i] == '\0')
		return (flush_errors("", 210, ""), -1);
	if (q->flag_sep == 0 && one_of_char(s_line[q->i], "&,|,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if (q->flag_sep == 1 && one_of_char(s_line[q->i], "&,|") != TRUE)
		return (flush_errors("", 210, ""), -1);
	if (q->flag_sep == 1)
	{
		if (s_line[q->i] == '|' && s_line[q->i + 1] == '|')
			sh->tk = 1;
		while (one_of_char(s_line[q->i], "&,|") == TRUE)
		{
			if (q->k == 0 && s_line[q->i] != s_line[q->i + 1])
				return (flush_errors("", 210, ""), -1);
			else if (q->k == 2)
				return (flush_errors("", 210, ""), -1);
			ft_memset(s_line + q->i, ' ', 1);
			q->k++;
			q->i++;
		}
	}
	return (0);
}

static int	parse_brackets(t_shell *sh, char *s_line, t_parse *q)
{
	while (s_line[q->i] != ')' && s_line[q->i] != '\0')
	{
		if (s_line[q->i] == '(')
			return (flush_errors("", 210, ""), -1);
		else if (s_line[q->i] == '(')
		{
			parse_subshell(sh, s_line, q);
			if (s_line[q->i] == '\0')
				return (0);
		}
		q->i++;
	}
	while (s_line[q->i] == ')')
	{
		if (s_line[q->i] == '(')
			return (flush_errors("", 210, ""), -1);
		ft_memset(s_line + q->i, ' ', 1);
		q->i++;
		q->flag_jump = 1;
		sh->bracket[1]++;
	}
	return (0);
}

static int	parse_subshell(t_shell *sh, char *s_line, t_parse *q)
{
	while (s_line[q->i] == ' ' && s_line[q->i] != '\0')
		q->i++;
	while (s_line[q->i] == '(')
	{
		ft_memset(s_line + q->i, ' ', 1);
		q->i++;
		q->flag_sep = 0;
		sh->bracket[0]++;
	}
	if (set_priority(sh, s_line, q) == -1)
		return (-1);
	if (parse_brackets(sh, s_line, q) == -1)
		return (-1);
	if (q->flag_jump == 1)
	{
		if (q->j > 19)
			return (flush_errors("", 210, ""), -1);
		q->i += sh_skip(s_line + q->i, ' ');
		if (s_line[q->i] == '\0')
			return (0);
		else
			return (flush_errors("", 210, ""), -1);
	}
	return (0);
}

char	*get_next_subshell(t_shell *sh, char *line, int *pos)
{
	t_parse	q;
	char	*res;

	init_parse(&q);
	q.i = *pos;
	if (*pos != 0)
		q.flag_sep = 1;
	q.beg_sep = *pos;
	q.i = search_next_token(sh, line, q.i + 2);
	if (q.i == -1)
		return (NULL);
	res = ft_substr(line, *pos, q.i - q.beg_sep);
	res = sh_trim_spaces(res);
	if (res == NULL)
		return (flush_errors("", 202, ""), NULL);
	*pos = q.i;
	q.i = 0;
	if (parse_subshell(sh, res, &q) == -1)
		return (free_s(res), NULL);
	return (res);
}
