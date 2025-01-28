/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:50:58 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 16:06:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_sh_redirs(char *t_line, t_parse *q)
{
	int	n;

	q->k = 0;
	n = 0;
	q->prev_pos2 = q->i;
	while (oneofchar(t_line[q->i], "<,>") != TRUE && t_line[q->i] != '\0')
		q->i++;
	if (t_line[q->i] == '\0')
		return (0);
	while (oneofchar(t_line[q->i], "<,>") == TRUE)
	{
		if (q->k >= 2)
			return (flush_errors("", 210, t_line[q->i]), -1);
		if (t_line[q->i] == '>' && t_line[q->i + 1] == '<')
			return (flush_errors("", 210, t_line[q->i]), -1);
		else if (t_line[q->i] == '<' && t_line[q->i + 1] == '>')
			return (flush_errors("", 210, t_line[q->i]), -1);
		q->i++;
		q->k++;
	}
	if (q->i > q->prev_pos2)
		n++;
	if (t_line[q->i] != '\0')
		n += count_sh_redirs(t_line, q);
	return (n);
}

static int	get_redirs_loop(t_shell *sh, char *t_line, t_parse *q)
{
	char	c;

	while (oneofchar(t_line[q->i], "<,>") == TRUE)
		q->i++;
	c = t_line[q->i - 1];
	q->i += sh_skip(t_line + q->i, ' ');
	if (t_line[q->i] == '\0')
		return (flush_errors("", 210, c), -1);
	sh->r->rd[q->j] = ft_substr(t_line, q->prev_pos, q->i - q->prev_pos);
	ft_memset(t_line + q->prev_pos, ' ', q->i - q->prev_pos);
	sh->r->rd[q->j] = sh_trim_spaces(sh->r->rd[q->j]);
	if (sh->r->rd[q->j] == NULL)
		return (flush_errors("", 202, 0), -1);
	q->prev_pos = q->i;
	while (t_line[q->i] != '\0' && t_line[q->i] != '>' && t_line[q->i] != '<')
		q->i++;
	sh->r->redirs[q->j] = ft_substr(t_line, q->prev_pos, q->i - q->prev_pos);
	ft_memset(t_line + q->prev_pos, ' ', q->i - q->prev_pos);
	sh->r->redirs[q->j] = sh_trim_spaces(sh->r->redirs[q->j]);
	if (sh->r->redirs[q->j] == NULL)
		return (flush_errors("", 202, 0), -1);
	if (sh->r->redirs[q->j++][0] == '\0')
		return (flush_errors("", 210, c), -1);
	return (0);
}

int	get_subshell_redirs(t_shell *sh, char *t_line, int *pos)
{
	t_parse	q;

	init_parse(&q);
	q.i = *pos;
	q.prev_pos = q.i;
	q.len = count_sh_redirs(t_line, &q);
	if (q.len == -1)
		return (-1);
	sh->r = (t_redirs *)ft_calloc(sizeof (t_redirs), 1);
	sh->r->rd = (char **)ft_calloc(sizeof(char *), q.len + 1);
	if (sh->r->rd == NULL || sh->r->rd == NULL)
		return (flush_errors("", 202, 0), -1);
	sh->r->redirs = (char **)ft_calloc(sizeof(char *), q.len + 1);
	if (sh->r->rd == NULL)
		return (flush_errors("", 202, 0), -1);
	q.i = q.prev_pos;
	while (t_line[q.i] != '\0' && oneofchar(t_line[q.i], "&,|,(,)") != TRUE)
	{
		if (get_redirs_loop(sh, t_line, &q) == -1)
			return (-1);
		if (t_line[q.i] == '\0')
			return (0);
	}
	return (0);
}
