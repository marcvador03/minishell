/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 15:30:46 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	skip_quotes(char *str);

char	*get_rd(char *line)
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
				return (">>");
			else
				return (">");
		}
		else if (line[i] == '<')
		{
			if (line[i + 1] == '<')
				return ("<<");
			else
				return ("<");
		}
		i++;
	}
	return (NULL);
}

int	count_redir(char *line)
{
	int		n;
	char	*rd;

	n = 0;
	if (line == NULL)
		return (n);
	rd = get_rd(line);
	if (rd == NULL)
		return (n);
	else if (rd != NULL)
		n += count_redir(line + sh_strpos(line, rd) + ft_strlen(rd));
	return (++n);
}

int	set_rd_flag(char *rd)
{
	if (ft_strncmp(rd, ">", ft_strlen(rd)) == 0)
		return (0);
	else if (ft_strncmp(rd, ">>", ft_strlen(rd)) == 0)
		return (2);
	else if (ft_strncmp(rd, "<", ft_strlen(rd)) == 0)
		return (1);
	else if (ft_strncmp(rd, "<<", ft_strlen(rd)) == 0)
		return (3);
	return (255);
}

/*char	*get_redirs(t_pipe *p, char **line, int *rd)
{
	int		i;
	int		n;
	char	*redirs;
	char	*t_rd;
	int		pos[2];

	n = count_redir(*line);
	i = 0;
	while (i < n)
	{
		t_rd = get_rd(*line);
		p->rd = set_rd_flag(t_rd);
		pos[0] = sh_strpos(*line, t_rd) + ft_strlen(t_rd);
		pos[1] = sh_strpos(ft_strtrim(*line + pos[0], " "), " ") + 1;
		redirs[i] = sh_strcut(*line, pos[0], pos[0] + pos[1]);
		*line = sh_strstrip(line, pos[0] - ft_strlen(t_rd), pos[0] + pos[1]);
		*line = sh_strtrim(line, t_rd, 0);
		i++;
	}
	redirs[n] = NULL;
	if (n > 0)
		sh_trim_list_strings(redirs, " ");
	return (redirs);
}*/

char	**create_redirs(t_pipe *p)
{
	int		i;
	char	**redirs;
	int		n;
	char	*t_rd;
	int		pos[2];
	
	p->p_line = sh_strtrim(&p->p_line, " ", 0);
	n = count_redir(p->p_line);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	p->rd = (int *)ft_calloc(sizeof(int), n);
	if (redirs == NULL || p->rd == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		t_rd = get_rd(p->p_line);
		p->rd[0] = set_rd_flag(t_rd);
		pos[0] = sh_strpos(p->p_line, t_rd) + ft_strlen(t_rd);
		pos[1] = sh_strpos(ft_strtrim(p->p_line + pos[0], " "), " ") + 1;
		redirs[i] = sh_strcut(p->p_line, pos[0], pos[0] + pos[1]);
		redirs[i] = sh_strtrim(&redirs[i], " ", 0);
		p->p_line = sh_strstrip(&p->p_line, pos[0] - ft_strlen(t_rd), pos[0] + pos[1]);
		p->p_line = sh_strtrim(&p->p_line, t_rd, 0);
i++;
	}
	redirs[n] = NULL;
	return (set_flag(p, 4), redirs);
}
//https://unix.stackexchange.com/questions/235092/command-redirection-to-multiple-files-command-file1-file2
