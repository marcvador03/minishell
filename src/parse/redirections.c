/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/20 11:27:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		find_next_token(char *line);
int			get_rd_flag(char *rd);
char	*expand_variable(t_pipe *p, char *line, int *i);
char	**get_sep_quotes(char *line);
char	*trim_line_expand(t_pipe *p, char *line);
int		get_fds_redir(t_pipe *p);
int		close_redir_fd_mult(t_pipe *p);

/*static char	*get_rd_check(char *line, char *res)
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
}*/


/*static char	*get_rd(char *line)
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
}*/

/*static int	count_redir(char *line)
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
}*/

/*static char	*create_redir_init(t_pipe *p, int i, char *line)
{
	char	*t_redirs[2];
	int		len[2];
	char	c;

	p->rd[i] = get_rd(line);
	len[0] = ft_strlen(p->rd[i]);
	t_redirs[0] = ft_strnstr(line, p->rd[i], ft_strlen(line));
	ft_memset(t_redirs[0], ' ', len[0]);
	t_redirs[0] = t_redirs[0] + sh_skip(t_redirs[0], ' ');
	if (ft_strlen(t_redirs[0]) == 0)
		return (flush_errors("", 203), NULL);
	c = find_next_token(t_redirs[0] + len[0]);
	len[1] = ft_strchr(t_redirs[0], c) - t_redirs[0];
	t_redirs[1] = ft_substr(line, t_redirs[0] - line, len[1]);
	if (t_redirs[1] == NULL)
		return (flush_errors("", -1), NULL);
	ft_memset(t_redirs[0], ' ', len[1]);
	p->p_line = line;
	if (sh_check_empty(t_redirs[1]) == -1)
		return (flush_errors("", 203), free_s(t_redirs[1]), NULL);
	if (get_rd_flag(p->rd[i]) != 4)
		t_redirs[1] = expand_variable(p, t_redirs[1], 0);
	return (t_redirs[1]);
}*/

int	count_redirs(char **parse, int *cnt_args)
{
	int		i;
	int		j;
	char	c;
	int		cnt;

	i = 0;
	cnt = 0;
	while (parse[i] != NULL)
	{
		j = 0;
		c = parse[i][j];
		if (c == '>' || c == '<')
		{
			while (parse[i][++j] != '\0')
			{
				if (j >= 2)
					return (flush_errors("parse[i][j]", 203), -1);
				if (parse[i][j] != c)
					return (flush_errors("parse[i][j]", 203), -1);
			}
			if (parse[++i] == NULL)
				return (flush_errors("parse[i][j]", 203), -1);
			else if (parse[i][0] == '>' || parse[i][0] == '<')
				return (flush_errors("parse[i][j]", 203), -1);
			cnt++;
		}
		else
			*cnt_args = *cnt_args + 1;
		i++;
	}
	return (cnt);
}

int	create_parsing(t_pipe *p)
{
	char	**parse;
	char	*tmp;
	int		i;
	int		j;
	int		k;
	int		cnt_redirs;
	int		cnt_args;

	tmp = p->p_line;
	p->p_line = ft_strtrim(p->p_line, " ");
	free_s(tmp);
	parse = get_sep_quotes(p->p_line);
	if (parse == NULL)
		return (flush_errors("", 202), 2);
	cnt_args = 0;
	cnt_redirs = count_redirs(parse, &cnt_args);
	if (cnt_redirs == -1)
		return (free_d(parse), 2);
	p->redirs = (char **)ft_calloc(sizeof(char *), cnt_redirs + 1);
	p->rd = (char **)ft_calloc(sizeof(char *), cnt_redirs + 1);
	p->args = (char **)ft_calloc(sizeof(char *), cnt_args + 1);
	if (p->redirs == NULL || p->rd == NULL || p->args == NULL)
		return (free_d(parse), flush_errors("", 202), 2);
	i = 0;
	j = 0;
	k = 0;
	while (parse[i] != NULL)
	{
		if (parse[i][0] == '>' ||  parse[i][0] == '<')
		{
			p->rd[j] = parse[i++];
			if (p->rd[j] == NULL)
				return (free_d(parse), flush_errors("", 202), 2);
			p->redirs[j] = trim_line_expand(p, parse[i]);
			if (p->redirs[j++] == NULL)
				return (free_d(parse), flush_errors("", 202), 2);
		}
		else
		{
			p->args[k] = trim_line_expand(p, parse[i]);
			if (p->args[k++] == NULL)
				return (free_d(parse), flush_errors("", 202), 2);
		}
		i++;
	}
	if (p->args[0] != NULL)
	{
		sh_updateenv(p->sh->env, ft_strdup("_"), ft_strdup((p->args)[0]));
		if (ft_strncmp(p->args[0], "exit", max(ft_strlen(p->args[0]), 4)) == 0)
			p->exit = 1;
	}
	if (get_fds_redir(p) == -1)
		return (close_redir_fd_mult(p->head), 2);
	return (free(parse), 0);
}

/*char	**create_redirs(t_pipe *p)
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
		redirs[i] = sh_trim_strings(redirs[i]);
		if (redirs[i] == NULL)
			return (free_d(redirs), NULL);
	}
	return (redirs);
}*/
