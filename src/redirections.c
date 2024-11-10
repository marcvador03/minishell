/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:52:08 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/11 00:40:35 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_redir(char *line)
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
	rd = get_redir(line);
	if (rd != NULL)
		n += count_redir(line + sh_strpos(line, rd) + ft_strlen(rd));
	else
		return (++n);
	return (0);
}

void	redirections(char *line, int *out_fd)
{
int		i;
	int		n;
	char	**sub_redir;
	char	*rd;
	int		*fd;
	int		err;
	char	c;

	n = count_redir(line);
	err = 0;
	sub_redir = (char **)ft_calloc(sizeof(char *), n);
	if (sub_redir == NULL)
		return;
	i = 0;
	//just get first test following > or <
	//how to repass remainings args (before next < or > to arg list)?
	//if > --> replace write of parent_fork to std_out sino replace ??
	//open redirections with R/W, manage permissions and APPEND for >> 


	while (i < n)
	{
		rd = get_redir(line);
		sub_redir[i] = sh_strcut(line, 0, sh_strpos(line, rd));
		if (sub_redir[i] == NULL)
			return ;
		line = line + sh_strpos(line, rd) + ft_strlen(rd);	
		i++;
	}
	sub_redir[n] = NULL;
	fd = (int *)ft_calloc(sizeof(int), n);
	if (fd == NULL)
		return;
	i = 0;
	while (i < n)
	{
		
	}

		
}

//https://unix.stackexchange.com/questions/235092/command-redirection-to-multiple-files-command-file1-file2
