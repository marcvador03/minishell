/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/11 17:26:47 by mfleury          ###   ########.fr       */
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
	return (" ");
}

int	count_redir(char *line)
{
	int		n;
	char	*rd;

	n = 0;
	if (line == NULL)
		return (n);
	rd = get_rd(line);
	if (ft_strncmp(rd, " ", 1) != 0)
		n += count_redir(line + sh_strpos(line, rd) + ft_strlen(rd));
	else
		return (++n);
	return (0);
}

char	**get_redirs(char *line)
{
	int		i;
	int		n;
	char	**redirs;
	char	*rd;
	int		pos[2];

	n = count_redir(line);
	redirs = (char **)ft_calloc(sizeof(char *), n + 1);
	if (redirs == NULL)
		return (NULL);
	i = 0;
	while (i <= n)
	{
		rd = get_rd(line);
		pos[0] = sh_strpos(line, rd) + 1;
		pos[1] = sh_strpos(line + pos[0], " ");
		redirs[i] = sh_strcut(line, pos[0], pos[1]);
		i++;
	}
	redirs[n] = NULL;
	return (redirs);		
	
	//just get first test following > or <
	//how to repass remainings args (before next < or > to arg list)?
	//if > --> replace write of parent_fork to std_out sino replace ??
	//open redirections with R/W, manage permissions and APPEND for >> 
}
	//https://unix.stackexchange.com/questions/235092/command-redirection-to-multiple-files-command-file1-file2
