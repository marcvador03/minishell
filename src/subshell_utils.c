/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/01 21:29:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	**create_fpipes(t_pipe *p)
{
	int	i;
	int	**fd;

	fd = (int **)ft_calloc(sizeof(int *), p->count + 1);
	if (fd == NULL)
		return (set_errno(ENOMEM), NULL);
	i = 0;
	while (i < p->count)
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
	return (set_flag(p, 4), fd);
}

char	***create_args(t_pipe *p)
{
	char	***args;
	int		i;

	args = (char ***)ft_calloc(sizeof(char **), p->count + 1);
	if (args == NULL)
		return (set_errno(ENOMEM), NULL);
	i = 0;
	while (i < p->count)
	{
		args[i] = get_cmd_args(p->in_pipes[i]);
		if (args[i] == NULL)
			return (free_d((void **)args[i]), set_errno(ENOMEM), NULL);
		i++;
	}
	return (set_flag(p, 3), args);
}

pid_t	*create_pids(t_pipe *p)
{
	pid_t	*pid;

	pid = (pid_t *)ft_calloc(sizeof(pid_t), p->count);
	if (pid == NULL)
		return (set_errno(ENOMEM), NULL);
	return (set_flag(p, 5), pid);
}
