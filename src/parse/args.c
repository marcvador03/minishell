/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:52:16 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/05 16:12:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char *line)
{
	int	n;
	int	i;

	if (line == NULL)
		return (0);
	n = 1;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			break ;
		if (line[i] == ' ')
		{
			i += sh_skip(line, ' ');
			n++;
		}
		i++;
	}
	return (n);
}

static char	*get_args(char **line)
{
	int		i;
	char	*res;

	if (line == NULL)
		return (NULL);
	i = 0;
	res = NULL;
	//sh_trim_strings(*line);
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == 34 || (*line)[i] == 39)
			i += sh_jump_to(*line + i, (*line)[i]);
		if ((*line)[i] == ' ')
		{
			i += sh_skip(*line + i, ' ');
			res = sh_strcut2(line, 0, i);
			return (res);
		}
		i++;
	}
	return (*line);
}

char	**create_args(t_pipe *p)
{
	char	**args;
	int		i;
	int		n;

	p->p_line = sh_strtrim(p->p_line, " ", 0);
	n = count_args(p->p_line);
	args = (char **)ft_calloc(sizeof(char *), n + 1);
	if (args == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		p->p_line = sh_strtrim(p->p_line, " ", 0);
		args[i] = get_args(&p->p_line);
		args[i] = sh_strtrim(args[i], " ", 0);
		sh_trim_strings(args[i]); 
		if (sh_check_empty(args[i]) != 0)
			free_s(args[i]);
		else
			i++;
	}
	args[n] = NULL;
	return (set_flag(p, 2), args);
}
