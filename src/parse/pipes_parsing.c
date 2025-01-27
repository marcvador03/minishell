/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/27 19:31:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_expand(t_redirs *r, char *line, int f_exp);
char	**get_sep_quotes(t_pipe *p);

static int	count_redirs_loop(char **parse, t_parse *r, char c)
{
	while (parse[r->i][++r->j] != '\0')
	{
		if (r->j >= 2)
			return (flush_errors("", 210, c), -1);
		if (parse[r->i][r->j] != c)
			return (flush_errors("", 210, c), -1);
	}
	if (parse[++r->i] == NULL)
		return (flush_errors("", 210, c), -1);
	else if (parse[r->i][0] == '>' || parse[r->i][0] == '<')
		return (flush_errors("", 210, c), -1);
	return (0);
}

static int	count_redirs(char **parse, int *cnt_args, int *cnt_redirs)
{
	t_parse	r;
	char	c;

	init_parse(&r);
	while (parse[r.i] != NULL)
	{
		r.j = 0;
		c = parse[r.i][r.j];
		if (c == '>' || c == '<')
		{
			if (count_redirs_loop(parse, &r, c) == -1)
				return (-1);
			*cnt_redirs = *cnt_redirs + 1;
		}
		else
			*cnt_args = *cnt_args + 1;
		r.i++;
	}
	return (0);
}

static int	close_parsing(t_pipe *p)
{
	if (p->args[0] != NULL)
	{
		sh_updateenv(p->sh->env, ft_strdup("_"), ft_strdup((p->args)[0]));
		if (ft_strncmp(p->args[0], "exit", max(ft_strlen(p->args[0]), 4)) == 0)
			p->exit = 1;
	}
	if (get_fds_redir(p->r, &p->p_status) == -1)
		return (close_redir_fd_pipe(p->head), 2);
	return (0);
}

static int	create_parsing_loop(t_pipe *p, t_parse *q)
{
	while (q->parse[q->i] != NULL)
	{
		if (q->parse[q->i][0] == '>' || q->parse[q->i][0] == '<')
		{
			p->r->rd[q->j] = q->parse[q->i++];
			if (p->r->rd[q->j] == NULL)
				return (flush_errors("", 202, 0), 2);
			q->flag_sep = get_rd_flag(p->r->rd[q->j]);
			p->r->redirs[q->j] = trim_expand(p->r, q->parse[q->i], q->flag_sep);
			if (p->r->redirs[q->j++] == NULL)
				return (flush_errors("", 202, 0), 2);
		}
		else
		{
			p->args[q->k] = trim_expand(p->r, q->parse[q->i], 0);
			if (p->args[q->k] != NULL)
				q->k++;
		}
		q->i++;
	}
	return (0);
}

int	create_parsing(t_pipe *p)
{
	t_parse	q;
	int		cnt_redirs;
	int		cnt_args;

	init_parse(&q);
	q.parse = get_sep_quotes(p);
	if (q.parse == NULL)
		return (2);
	cnt_args = 0;
	cnt_redirs = 0;
	q.flag_jump = count_redirs(q.parse, &cnt_args, &cnt_redirs);
	if (q.flag_jump == -1)
		return (free_d(q.parse), 2);
	p->r->redirs = (char **)ft_calloc(sizeof(char *), cnt_redirs + 1);
	p->r->rd = (char **)ft_calloc(sizeof(char *), cnt_redirs + 1);
	p->args = (char **)ft_calloc(sizeof(char *), cnt_args + 1);
	if (p->r->redirs == NULL || p->r->rd == NULL || p->args == NULL)
		return (free_d(q.parse), flush_errors("", 202, 0), 2);
	if (create_parsing_loop(p, &q) == 2)
		return (free(q.parse), 2);
	if (close_parsing(p) == 2)
		return (free(q.parse), 2);
	return (free(q.parse), 0);
}
