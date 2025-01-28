/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parsing_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 16:10:10 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 16:13:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_token(t_shell *sh, char *line, t_parse *q, int *l_status);
int	inside_bracket(t_shell *sh, char *line, t_parse *q, int *l_status);

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

static int	get_next_rd(char *line, t_parse *q, int *l_status)
{
	q->prev_pos = q->i;
	while (oneofchar(line[q->i], "&,|,(,)") == FALSE && line[q->i] != '\0')
		q->i++;
	if (line[q->i] == '(')
	{
		*l_status = flush_errors("", 210, line[q->prev_pos]);
		return (-1);
	}
	return (0);
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

int	get_next_token(t_shell *sh, char *line, t_parse *q, int *l_status)
{
	if (sh != sh->head)
		set_priority(sh, line, q);
	while (oneofchar(line[q->i], "&,|,(,),<,>") == FALSE && line[q->i] != '\0')
		q->i++;
	if (line[q->i] == '\0')
		return (q->i);
	q->tk = line[q->i];
	if (line[q->i] == '>' || line[q->i] == '<')
	{
		if (get_next_rd(line, q, l_status) == -1)
			return (-1);
	}
	else if (line[q->i] == '&' || line[q->i] == '|')
	{
		if (get_next_dollar(sh, line, q, l_status) == -1)
			return (-1);
	}
	else if (line[q->i] == '(')
		return (inside_bracket(sh, line, q, l_status));
	return (q->i);
}
