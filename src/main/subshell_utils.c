/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 15:29:42 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*int	*create_fd_pipes(t_pipe *p)
{
	int	i;
	int	*fd;

	fd = (int *)ft_calloc(sizeof(int), p->count);
	if (fd == NULL)
		return ( NULL);
	i = 0;
	while (i < p->count - 1)
	{
		fd[i] = (int *)ft_calloc(sizeof(int), 2);
		if (fd[i] == NULL)
			return (free_d((void **)fd), set_errno(ENOMEM), NULL);
		if (pipe(fd[i++]) == -1)
		{
			fd[i] = NULL;
			return (free_d((void **)fd), NULL);
		}
	}
	fd[i] = NULL;
	return (set_flag(p, 2), fd);
}*/

int	count_args(char *line)
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
			break;
		if (line[i] == ' ')
		{
			i += sh_skip(line, ' ');
			n++;
		}
		i++;
	}
	return (n);
}

char	*get_args(char **line)
{
	int		i;
	int		pos;
	char	*res;

	if (line == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	res = NULL;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == 34 || (*line)[i] == 39)
			pos = sh_jump_to(*line + i, (*line)[i]);
		else if ((*line)[i] == ' ')
			pos = i + sh_skip(*line + i, ' ');
		if (pos > 0)
		{
			res = sh_strcut2(line, 0, pos);
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

	p->p_line = sh_strtrim(&p->p_line, " ", 0);
	n = count_args(p->p_line);
	args = (char **)ft_calloc(sizeof(char *), n + 1);
	if (args == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		p->p_line = sh_strtrim(&p->p_line, " ", 0);
		args[i] = get_args(&p->p_line);
		args[i] = sh_strtrim(&args[i], " ", 0);
		i++;
	}
	args[n] = NULL;
	return (set_flag(p, 1), args);
}


/*pid_t	*create_pids(t_pipe *p)
{
	pid_t	*pid;

	pid = (pid_t *)ft_calloc(sizeof(pid_t), p->count);
	if (pid == NULL)
		return (set_errno(ENOMEM), NULL);
	return (set_flag(p, 3), pid);
}*/
