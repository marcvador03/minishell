/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 16:25:14 by mfleury          ###   ########.fr       */
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
			i = i + sh_skip(line + i, ' ');
			if (line[i] == '<' || line [i] == '>')
				return (NULL);
			else
				return (res);
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
		while (line[i] == 34 || line[i] == 39)
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
		else if (line[i++] == '\0')
			return ("");
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

static char	*create_redir_init(t_pipe *p, int i, char *line)
{
	char	*t_redirs[2];
	int		len[2];

	p->rd[i] = get_rd(line);
	len[0] = ft_strlen(p->rd[i]);
	t_redirs[0] = ft_strnstr(line, p->rd[i], ft_strlen(line));
	ft_memset(t_redirs[0], ' ', len[0]);
	t_redirs[0] = t_redirs[0] + sh_skip(t_redirs[0], ' ');
	len[1] = ft_strchr(t_redirs[0], ' ') - t_redirs[0];
	t_redirs[1] = ft_substr(line, t_redirs[0] - line, len[1]);
	if (t_redirs[1] == NULL)
		return (flush_errors("", -1), NULL);
	ft_memset(t_redirs[0], ' ', len[1]);
	p->p_line = line;
	if (sh_check_empty(t_redirs[1]) == -1)
		return (flush_errors("", 203), free_s(t_redirs[1]), NULL);
	return (t_redirs[1]);
}

char	**create_redirs(t_pipe *p, t_env *env)
{
	int		i;
	char	**redirs;
	char	*t_line[2];
	int		n;

	t_line[0] = p->p_line + sh_skip(p->p_line, ' ');
	n = count_redir(t_line[0]);
	if (n == -1)
		return (flush_errors("", 203), NULL);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	p->rd = (char **)ft_calloc(sizeof(char *), n + 1);
	if (redirs == NULL || p->rd == NULL)
		return (free_s(redirs), flush_errors("", 202), NULL);
	i = -1;
	while (++i < n)
	{
		redirs[i] = create_redir_init(p, i, t_line[0]);
		if (redirs[i] == NULL)
			return (free_d(redirs), NULL);
		redirs[i] = expand_env(env, redirs[i], 1, p->sh->l_status);
		redirs[i] = sh_trim_strings(redirs[i]);
		if (redirs[i] == NULL)
			return (free_d(redirs), NULL);
	}
	return (redirs);
}
