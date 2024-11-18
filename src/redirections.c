/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/18 17:05:28 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_rd(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
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

char	**get_redirs(char **line, int **rd)
{
	int		i;
	int		n;
	char	**redirs;
	char	*t_rd;
	int		pos[2];

	n = count_redir(*line);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	*rd = (int *)ft_calloc(sizeof(int), n);
	if (redirs == NULL || rd == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		t_rd = get_rd(*line);
		pos[0] = sh_strpos(*line, t_rd) + ft_strlen(t_rd);
		pos[1] = sh_strpos(ft_strtrim(*line + pos[0], " "), " ") + 1;
		redirs[i] = sh_strcut(*line, pos[0], pos[0] + pos[1]);
		rd[0][i++] = set_rd_flag(t_rd);
		*line = sh_strstrip(line, pos[0] - ft_strlen(t_rd), pos[0] + pos[1]);
		*line = sh_strtrim(line, t_rd, 0);
	}
	redirs[n] = NULL;
	clean_spaces(redirs);
	return (redirs);
}
	//https://unix.stackexchange.com/questions/235092/command-redirection-to-multiple-files-command-file1-file2
