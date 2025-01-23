/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_parsing_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:50:58 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 18:31:20 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_sh_redirs(char *t_line, t_parse *q)
{
	int	n;
		
	q->k = 0;
	n = 0;
	q->prev_pos2 = q->i;
	while (one_of_char(t_line[q->i], "<,>") != TRUE && t_line[q->i] != '\0')
		q->i++;
	if (t_line[q->i] == '\0')
		return (0);
	while (one_of_char(t_line[q->i], "<,>") == TRUE)
	{
		if (q->k >= 2)
			return (flush_errors("", 210, ""), -1);
		if (t_line[q->i] == '>' && t_line[q->i + 1] == '<')
			return (flush_errors("", 210, ""), -1);
		else if (t_line[q->i] == '<' && t_line[q->i + 1] == '>')
			return (flush_errors("", 210, ""), -1);
		q->i++;
		q->k++;
	}
	if (q->i > q->prev_pos2)
		n++;
	if (t_line[q->i] != '\0')
		n += count_sh_redirs(t_line, q);
	return (n);
}

int	get_subshell_redirs(t_shell *sh, char **s_line, int *pos)
{
	char	*t_line;
	t_parse	q;

	init_parse(&q);
	t_line = *s_line;
	q.i = *pos;
	/*while (one_of_char(t_line[q.i], "<,>") != TRUE)
	{	
		if (t_line[q.i] != '\0')
			return (0);
		if (one_of_char(t_line[q.i], "&,|") == TRUE)
			return (0);
		if (t_line[q.i] == '(')
			return (flush_errors("", 210, ""), -1);
		q.i++;
	}*/
	q.prev_pos = q.i;
	q.len = count_sh_redirs(t_line, &q);
	if (q.len == -1)
		return (-1);	
	sh->r = (t_redirs *)ft_calloc(sizeof (t_redirs), 1);
	if (sh->r == NULL)
		return (flush_errors("", 202, ""), -1);
	sh->r->rd = (char **)ft_calloc(sizeof(char *), q.len + 1);
	if (sh->r->rd == NULL)
		return (flush_errors("", 202, ""), -1);
	sh->r->redirs = (char **)ft_calloc(sizeof(char *), q.len + 1);
	if (sh->r->rd == NULL)
		return (flush_errors("", 202, ""), -1);
	q.i = q.prev_pos;
	while (t_line[q.i] != '\0' && one_of_char(t_line[q.i], "&,|,(,)") != TRUE)
	{
		while (one_of_char(t_line[q.i], "<,>") == TRUE)
			q.i++;
		q.i += sh_skip(t_line + q.i, ' ');
		if (t_line[q.i] == '\0')
			return (flush_errors("", 210, ""), -1);
		sh->r->rd[q.j] = ft_substr(t_line, q.prev_pos, q.i - q.prev_pos);
		ft_memset(t_line + q.prev_pos, ' ', q.i - q.prev_pos);
		sh->r->rd[q.j] = sh_trim_spaces(sh->r->rd[q.j]);
		if (sh->r->rd[q.j] == NULL)
			return (flush_errors("", 202, ""), -1);
		q.prev_pos = q.i;
		while (t_line[q.i] != '\0' && t_line[q.i] != '>' && t_line[q.i] != '<')
			q.i++;
		sh->r->redirs[q.j] = ft_substr(t_line, q.prev_pos, q.i - q.prev_pos);
		ft_memset(t_line + q.prev_pos, ' ', q.i - q.prev_pos);
		sh->r->redirs[q.j] = sh_trim_spaces(sh->r->redirs[q.j]);
		if (sh->r->redirs[q.j] == NULL)
			return (flush_errors("", 202, ""), -1);
		if (sh->r->redirs[q.j++][0] == '\0')
			return (flush_errors("", 210, ""), -1);
		if (t_line[q.i] == '\0')
			return (0);
	}
	return (0);
}

/*int	get_subshell_redirs(t_shell *sh, char **s_line, t_parse *q)
{
	char	*t_line;

	t_line = *s_line;
	if (none_of_char(t_line[q->i], "<,>") == TRUE)
		return (flush_errors("", 210, ""), -1);
	q->prev_pos = q->i;
	q->len = count_sh_redirs(t_line, q);
	if (q->len == -1)
		return (-1);	
	sh->r->rd = (char **)ft_calloc(sizeof(char *), q->len + 1);
	if (sh->r->rd == NULL)
		return (flush_errors("", 202, ""), -1);
	sh->r->redirs = (char **)ft_calloc(sizeof(char *), q->len + 1);
	if (sh->r->rd == NULL)
		return (flush_errors("", 202, ""), -1);
	q->i = q->prev_pos;
	while (one_of_char(t_line[q->i], "<,>") == TRUE)
		q->i++;
	q->i += sh_skip(t_line + q->i, ' ');
	if (t_line[q->i] == '\0')
		return (flush_errors("", 210, ""), -1);
	sh->r->rd[q->j] = ft_substr(t_line, q->prev_pos, q->i - q->prev_pos);
	ft_memset(t_line + q->prev_pos, ' ', q->i - q->prev_pos);
	sh->r->rd[q->j] = sh_trim_spaces(sh->r->rd[q->j]);
	if (sh->r->rd[q->j] == NULL)
		return (flush_errors("", 202, ""), -1);
	q->prev_pos = q->i;
	while (t_line[q->i] != '\0' && t_line[q->i] != '>' && t_line[q->i] != '<')
		q->i++;
	sh->r->redirs[q->j] = ft_substr(t_line, q->prev_pos, q->i - q->prev_pos);
	ft_memset(t_line + q->prev_pos, ' ', q->i - q->prev_pos);
	sh->r->redirs[q->j] = sh_trim_spaces(sh->r->redirs[q->j]);
	if (sh->r->redirs[q->j] == NULL)
		return (flush_errors("", 202, ""), -1);
	if (sh->r->redirs[q->j][0] == '\0')
		return (flush_errors("", 210, ""), -1);
	sh->r->exist = 1;
	if (t_line[q->i] == '\0')
		return (0);
	else if (one_of_char(t_line[q->i], "<,>") == TRUE)
		return (get_subshell_redirs(sh, s_line, q), 0);
	return (0);
}*/
