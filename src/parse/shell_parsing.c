/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:43:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/04 09:47:46 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*sh_lstadd_back(t_shell *sh);
t_shell	*sh_lstadd_down(t_shell *sh);
t_shell	*parse_sh(t_shell *sh, char *line, int *pos, int *l_status);
int		get_subshell_redirs(t_shell *sh, char *t_line, int *pos);
int		get_next_token(t_shell *sh, char *line, t_parse *q, int *l_status);

static int	close_bracket(t_shell *sh, char *line, t_parse *q, int *l_status)
{
	ft_memset(line + q->i, ' ', 1);
	q->prev_pos = q->i;
	q->i += sh_skip(line + q->i, ' ');
	if (line[q->i] == '\0')
		return (0);
	else if (oneofchar(line[q->i], "<,>,&,|,(,)") != TRUE)
		return (set(flush_errors("", 210, q->tk), l_status), -1);
	if (line[q->i] == '\0')
		q->i = q->prev_pos;
	else if (line[q->i] == '>' || line[q->i] == '<')
	{
		if (get_subshell_redirs(sh, line, &q->i) == -1)
			return (-1);
	}
	else if (oneofchar(line[q->i], "<,>,&,|,(,)") != TRUE && line[q->i] != '\0')
		return (set(flush_errors("", 210, q->tk), l_status), -1);
	return (0);
}

static int	inside_bracket_errors(char *line, t_parse *q)
{
	int	i;

	i = q->i;
	while (i >= q->beg_sep)
	{
		if (oneofchar(line[i], "&,|") == TRUE)
			break ;
		if (oneofchar(line[i], " ,(") == FALSE)
			return (flush_errors("", 210, q->tk), -1);
		i--;
	}
	return (0);

}

int	inside_bracket(t_shell *sh, char *line, t_parse *q, int *l_status)
{
	t_shell	*sub_sh;

	q->tk = line[q->i];
	if (inside_bracket_errors(line, q) == -1)
		return (-1) ;
	q->i++;
	sub_sh = sh_lstadd_down(sh);
	if (sub_sh == NULL)
		return (-1);
	sub_sh = parse_sh(sub_sh, line, &q->i, l_status);
	if (sub_sh == NULL)
		return (-1);
	if (line[q->i] == ')')
	{
		if (close_bracket(sh, line, q, l_status) == -1)
			return (-1);
	}
	while (oneofchar(line[q->i], "&,|,(,)") != TRUE && line[q->i] != '\0')
		q->i++;
	return (0);
}

static int	parse_sh_errors(t_shell *sh, t_parse *q, int *l_status)
{
	if (sh->s_line == NULL)
	{
		*l_status = flush_errors("", 202, 0);
		return (-1);
	}
	if (sh->s_line[0] == '\0')
	{
		*l_status = flush_errors("", 210, q->tk);
		return (free_sh(sh), -1);
	}
	if (sh->r != NULL && sh->down == NULL)
	{
		*l_status = flush_errors("", 210, *sh->r->rd[0]);
		return (free_sh(sh), -1);
	}
	return (0);
}

t_shell	*parse_sh(t_shell *sh, char *line, int *pos, int *l_status)
{
	t_parse	q;

	init_parse(&q);
	q.i = *pos;
	while (line[q.i] != '\0' && line[q.i] != ')')
	{
		q.beg_sep = q.i;
		if (get_next_token(sh, line, &q, l_status) == -1)
			return (free_sh(sh), NULL);
		sh->s_line = ft_substr(line, q.beg_sep, q.i - q.beg_sep);
		sh->s_line = sh_trim_spaces(sh->s_line);
		if (parse_sh_errors(sh, &q, l_status) == -1)
			return (NULL);
		if (line[q.i] != '\0' && line[q.i] != ')')
			sh = sh_lstadd_back(sh);
	}
	*pos = q.i;
	if (line[q.i] == ')' && (sh->s_line == NULL || sh->s_line[0] == '\0'))
	{
		*l_status = flush_errors("", 210, line[q.i]);
		return (free_sh(sh), NULL);
	}
	return (sh);
}
