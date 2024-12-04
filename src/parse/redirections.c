/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/04 12:07:02 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_rd_check(char *line, char *res)
{
	int		i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '<' || line[i] == '>')
			return (NULL);
		else if (line[i] == ' ')
		{
			i = sh_skip(line, ' ');
			if (line[i] == '<' || line [i] == '>')
				return (NULL);
		}
		else
			return (res);
		i++;
	}
	return (res);
}

static char	*get_rd(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '>')
		{
			if (line[i + 1] == '>')
				return (get_rd_check(line + i + 2, ">>"));
			else 
				return (get_rd_check(line + i + 1, ">"));
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
				return (get_rd_check(line + i + 2, "<<"));
			else
				return (get_rd_check(line + i + 1, "<"));
		}
		i++;
	}
	return ("");
}

static int	count_redir(char *line)
{
	int		n;
	char	*rd;

	n = 0;
	if (line == NULL)
		return (n);
	rd = get_rd(line);
	if (sh_check_empty(rd) == -1)
		return (n);
	else if (rd == NULL)
		return (-1);
	else if (rd != NULL)
		n += count_redir(line + sh_strpos(line, rd) + ft_strlen(rd));
	return (++n);
}

static char	*create_redir_init(t_pipe *p, int i)
{
	int		pos[2];
	char	*s_redirs;
	int		len;

	p->rd[i] = get_rd(p->p_line);
	pos[0] = sh_strpos(p->p_line, p->rd[i]) + ft_strlen(p->rd[i]);
	pos[1] = sh_strpos(ft_strtrim(p->p_line + pos[0], " "), " ") + 1; //leak
	s_redirs = sh_strcut(p->p_line, pos[0], pos[0] + pos[1]);
	s_redirs = sh_strtrim(s_redirs, " ", 0);
	len = ft_strlen(p->rd[i]);
	p->p_line = sh_strstrip(&p->p_line, pos[0] - len, pos[0] + pos[1]);
	p->p_line = sh_strtrim(p->p_line, p->rd[i], 0);
	return (s_redirs);
}

char	**create_redirs(t_pipe *p)
{
	int		i;
	char	**redirs;
	int		n;

	p->p_line = sh_strtrim(p->p_line, " ", 0);
	n = count_redir(p->p_line);
	if (n == -1)
		return (set_gstatus(203), NULL);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	p->rd = (char **)ft_calloc(sizeof(char *), n + 1);
	if (redirs == NULL || p->rd == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		redirs[i] = create_redir_init(p, i);
		if (sh_check_empty(redirs[i]) == -1)
			return (set_gstatus(203), NULL);
		i++;
	}
	redirs[n] = NULL;
	p->rd[n] = NULL;
	return (set_flag(p, 1), redirs);
}
