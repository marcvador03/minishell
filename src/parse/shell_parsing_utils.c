/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parsing_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:10:10 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/04 18:34:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_token(t_shell *sh, char *line, t_parse *q, int *l_status);
int	inside_bracket(t_shell *sh, char *line, t_parse *q, int *l_status);

static void	set_priority(t_shell *sh, char *line, t_parse *q)
{
	char	c;

	if (line[q->i] == '\0')
		return ;
	q->i += sh_skip(line + q->i, ' ');
	if (line[q->i + 1] == '\0')
		return ;
	if (line[q->i] == '|' && line[q->i + 1] == '|')
		sh->tk = 1;
	if (line[q->i] == '&' || line[q->i + 1] == '|')
	{
		if (line[q->i + 1] == line[q->i])
		{
			c = line[q->i];
			while (line[q->i] == c)
			{
				ft_memset(line + q->i, ' ', 1);
				q->i++;
			}
		}
	}
	return ;
}

static int	check_tokens_errors(char *line, t_parse *q, int *l_status)
{
	if (line[q->i] == '&' && line[q->i] != line[q->i + 1])
	{
		*l_status = flush_errors("", 210, line[q->i]);
		return (-1);
	}
	else if (line[q->i + 1] != '\0')
	{
		if (oneofchar(line[q->i + 2], "&,|,)") == TRUE)
		{
			*l_status = flush_errors("", 210, line[q->i + 1]);
			return (-1);
		}
	}
	return (0);
}

static int	get_next_rd(t_shell *sh, char *line, t_parse *q, int *l_status)
{
	char	c;

	q->prev_pos = q->i;
	c = line[q->prev_pos];
	q->k = 0;
	while (line[q->i] == line[q->prev_pos])
	{
		q->k++;
		q->i++;
	}
	if (line[q->i] == '\0')
		return (set(flush_errors("", 210, c), l_status), -1);
	q->i += sh_skip(line + q->i, ' ');
	if (line[q->i] == '\0')
		return (set(flush_errors("", 210, c), l_status), -1);
	if (q->k > 2 || oneofchar(line[q->i], "&,|,(,)") == TRUE)
		return (set(flush_errors("", 210, c), l_status), -1);
	return (get_next_token(sh, line, q, l_status));
}

static int	get_next_dollar(t_shell *sh, char *line, t_parse *q, int *l_status)
{
	if (check_tokens_errors(line, q, l_status) == -1)
		return (-1);
	if (line[q->i + 1] == '&' || line[q->i + 1] == '|')
		return (0);
	else if (line[q->i] == '|')
	{
		if (noneofchar(line[++q->i], "|,&,(,)") == TRUE)
			return (get_next_token(sh, line, q, l_status));
		else
			q->i--;
	}
	return (0);
}

int	get_next_token(t_shell *sh, char *l, t_parse *q, int *l_status)
{
	if (sh != sh->head)
		set_priority(sh, l, q);
	while (oneofchar(l[q->i], "&,|,(,),<,>,',\"") == FALSE && l[q->i] != '\0')
		q->i++;
	if (l[q->i] == 34 || l[q->i] == 39)
	{
		q->i += sh_jump_to(l + q->i, l[q->i]);
		return (get_next_token(sh, l, q, l_status));
	}
	if (l[q->i] == '\0')
		return (q->i);
	q->tk = l[q->i];
	if (l[q->i] == '>' || l[q->i] == '<')
	{
		if (get_next_rd(sh, l, q, l_status) == -1)
			return (-1);
	}
	else if (l[q->i] == '&' || l[q->i] == '|')
	{
		if (get_next_dollar(sh, l, q, l_status) == -1)
			return (-1);
	}
	else if (l[q->i] == '(')
		return (inside_bracket(sh, l, q, l_status));
	return (q->i);
}
