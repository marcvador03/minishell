/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:43:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 19:50:10 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_priority(t_shell *sh, char *line, t_parse *q)
{
	char	c;

	q->i += sh_skip(line + q->i, ' ');
	if (line[q->i] == '|' && line[q->i] == '|')
		sh->tk = 1;
	c = line[q->i];
	while (line[q->i] == c)
	{
		ft_memset(line + q->i, ' ', 1);
		q->i++;
	}
	return ;
}

static int	get_inside_bracket(t_shell *sh, char *line, t_parse *q)
{
	t_shell	*sub_sh;

	q->i++;
	sub_sh = sh_lstadd_down(sh);
	if (sub_sh == NULL)
		return (-1);
	parse_sh(sub_sh, line, &q->i);
	if (line[q->i] == ')')
	{
		ft_memset(line + q->i, ' ', 1);
		q->prev_pos = q->i;
		while (one_of_char(line[q->i], "<,>") != TRUE && line[q->i] != '\0')
			q->i++;
		if (line[q->i] == '\0')
			q->i = q->prev_pos;
		else
			if (get_subshell_redirs(sh, &line, &q->i) == -1)
				return (-1);
	}
	while (one_of_char(line[q->i], "&,|,(,)") != TRUE && line[q->i] != '\0')
		q->i++;
	return (0);
}

static int	check_tokens_errors(char *line, t_parse *q)
{
	if (line[q->i] == '&' && line[q->i] != line[q->i + 1])
		return (flush_errors("", 210, ""), -1);
	else if (line[q->i + 1] != '\0')
	{
		if (one_of_char(line[q->i + 2], "&,|,)") == TRUE)
			return (flush_errors("", 210, ""), -1);
	}
	return (0);
}

static int	get_next_token(t_shell *sh, char *line, t_parse *q)
{
	if (sh != sh->head)
		set_priority(sh, line, q);
	while (one_of_char(line[q->i], "&,|,(,)") == FALSE && line[q->i] != '\0')
		q->i++;
	if (line[q->i] == '\0')
		return (q->i);
	else if (line[q->i] == '&' || line[q->i] == '|')
	{
		if (check_tokens_errors(line, q) == -1)
			return (-1);
		if (line[q->i + 1] == '&' || line[q->i + 1] == '|')
			return (0);
		else if (line[q->i] == '|')
		{
			if (none_of_char(line[q->i], "|,&,(,)") == TRUE)
				return (get_next_token(sh, line, q));
			else
				q->i--;
		}
	}
	else if (line[q->i] == '(')
		return (get_inside_bracket(sh, line, q));
	return (q->i);
}

t_shell	*parse_sh(t_shell *sh, char *line, int *pos)
{
	t_parse	q;

	init_parse(&q);
	q.i = *pos;
	while (line[q.i] != '\0' && line[q.i] != ')')
	{
		q.beg_sep = q.i;
		if (get_next_token(sh, line, &q) == -1)
			return (NULL);
		sh->s_line = ft_substr(line, q.beg_sep, q.i - q.beg_sep);
		sh->s_line = sh_trim_spaces(sh->s_line);
		if (sh->s_line == NULL)
			return (flush_errors("", 202, ""), NULL);
		if (sh->s_line[0] == '\0')
			return (flush_errors("", 210, ""), NULL);
		if (line[q.i] != '\0' && line[q.i] != ')')
			sh = sh_lstadd_back(sh);
	}
	*pos = q.i;
	return (sh);
}
