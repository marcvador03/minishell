/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/11 15:24:08 by mfleury          ###   ########.fr       */
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
		else if (line[i] == '\0')
			return ("");
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

/*static char	*create_redir_init(t_pipe *p, int i)
{
	int		pos[2];
	char	*s_redirs;
	char	*t_redirs;
	int		len;

	p->rd[i] = get_rd(p->p_line);
	pos[0] = sh_strpos(p->p_line, p->rd[i]) + ft_strlen(p->rd[i]);
	t_redirs = ft_strtrim(p->p_line + pos[0], " ");
	pos[1] = sh_strpos(t_redirs, " ") + 1;
	free_s(t_redirs);
	s_redirs = sh_strcut(p->p_line, pos[0], pos[0] + pos[1]);
	s_redirs = sh_strtrim(s_redirs, " ", 0);
	len = ft_strlen(p->rd[i]);
	p->p_line = sh_strstrip(&p->p_line, pos[0] - len, pos[0] + pos[1]);
	p->p_line = sh_strtrim(p->p_line, p->rd[i], 0);
	return (s_redirs);
}*/

static char	*create_redir_init(t_pipe *p, int i, char *line)
{
//	int		pos[2];
	char	*t_redirs[2];
	int		len[2];

	p->rd[i] = get_rd(line);
	len[0] = ft_strlen(p->rd[i]);
	t_redirs[0] = ft_strnstr(line, p->rd[i], ft_strlen(line));
	ft_memset(t_redirs[0], ' ', len[0]);
	t_redirs[0] = t_redirs[0] + sh_skip(t_redirs[0], ' ');
	len[1] = ft_strchr(t_redirs[0], ' ') - t_redirs[0];
	/*pos[0] = sh_strpos(line, p->rd[i]) + ft_strlen(p->rd[i]);
	t_redirs[0] = ft_strtrim(line + pos[0], " ");
	pos[1] = sh_strpos(t_redirs[0], " ") + 1;
	free_s(t_redirs[0]);*/
	//t_redirs[0] = ft_substr(line, pos[0], pos[1] - pos[0]);
	t_redirs[1] = ft_substr(line, t_redirs[0] - line, len[1]);
	//t_redirs[1] = ft_strtrim(t_redirs[0], " ");
	//len[1] = ft_strlen(t_redirs[0]);
	//free_s(t_redirs[0]);
	ft_memset(t_redirs[0],' ', len[1]);	
	//ft_memset(line + pos[0] - len[0],' ' , len[0] + len[1]);	
	/*if (p->p_line != line)
		free_s(p->p_line);*/
	p->p_line = line;
	return (t_redirs[1]);
}

char	**create_redirs(t_pipe *p)
{
	int		i;
	char	**redirs;
	char	*t_line[2];
	int		n;

//	t_line[0] = ft_strtrim(p->p_line, " ");
	t_line[0] = p->p_line + sh_skip(p->p_line, ' ');
//	t_line[1] = t_line[0];
	n = count_redir(t_line[0]);
	if (n == -1)
		return (set_gstatus(203), NULL);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	p->rd = (char **)ft_calloc(sizeof(char *), n + 1);
	if (redirs == NULL || p->rd == NULL)
		return (NULL);
	redirs[n] = NULL;
	p->rd[n] = NULL;
	if (n == 0)
		return (set_flag(p, 1), redirs);
	i = 0;
	while (i < n)
	{
		redirs[i] = create_redir_init(p, i, t_line[0]);
		//redirs[i] = ft_strtrim(t_redirs, " ");
		//free_s(t_redirs);
		redirs[i] = sh_trim_strings(redirs[i]); 
		if (sh_check_empty(redirs[i]) == -1)
			return (set_gstatus(203), NULL);
		i++;
	}
	return (set_flag(p, 1), redirs);
}

/*char	**create_redirs(t_pipe *p)
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
		//redirs[i] = ft_strtrim(t_redirs, " ");
		//free_s(t_redirs);
		redirs[i] = sh_trim_strings(redirs[i]); 
		if (sh_check_empty(redirs[i]) == -1)
			return (set_gstatus(203), NULL);
		i++;
	}
	redirs[n] = NULL;
	p->rd[n] = NULL;
	return (set_flag(p, 1), redirs);
}*/
