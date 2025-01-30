/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:41:25 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/30 18:59:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_separation(char *line, t_parse *q)
{
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

int	create_rd_separation(char *line, t_parse *q)
{
	if (q->i >= 1 && line[q->i - 1] != ' ')
	{
		q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
		q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
		if (q->parse[q->j++] == NULL)
			return (-1);
	}
	q->beg_sep = q->i;
	while (line[q->i] == '>' || line[q->i] == '<')
		q->i++;
	q->i += sh_skip(line + q->i, ' ');
	q->parse[q->j] = ft_substr(line, q->beg_sep, q->i - q->beg_sep);
	q->parse[q->j] = sh_trim_spaces(q->parse[q->j]);
	if (q->parse[q->j++] == NULL)
		return (-1);
	q->beg_sep = q->i;
	q->flag_jump = 1;
	if (line[q->i] == '\0')
		return (1);
	return (0);
}

int	separate_quotes(char *line, t_parse *q)
{
	q->i += sh_jump_to(line + q->i, line[q->i]);
	if (line[q->i] == '\0')
		return (1);
	while (noneofchar(line[q->i], " ,\",',>,<,$") == 1)
	{
		q->i++;
		if (line[q->i] == '\0')
			return (1);
	}
	if (line[q->i] == ' ')
	{
		q->i += sh_skip(line + q->i, ' ') - 1;
		q->flag_jump = 0;
	}
	return (0);
}

int	separate_dollar(char *line, t_parse *q)
{
	q->i++;
	if (line[q->i] == '\0')
		return (1);
	while (noneofchar(line[q->i], " ,>,<") == 1)
	{
		if (line[q->i] == 39 || line[q->i] == 34)
			q->i += sh_jump_to(line + q->i, line[q->i]);
		else
			q->i++;
		if (line[q->i] == '\0')
			return (1);
	}
	if (line[q->i] == '\0')
		return (1);
	q->i += sh_skip(line + q->i, ' ');
	q->flag_jump = 1;
	return (0);
}
