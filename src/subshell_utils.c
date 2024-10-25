/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 13:59:57 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:02:19 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	**create_fpipes(t_shell *sh)
{
	int	i;
	int	**fd;

	fd = (int **)ft_calloc(sizeof(int *), sh->count + 1);
	if (fd == NULL)
		return (set_errno(ENOMEM), NULL);
	i = 0;
	while (i < sh->count)
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
	return (set_flag(sh, 4), fd);
}

char	***create_args(t_shell *sh)
{
	char	***args;
	int		i;

	args = (char ***)ft_calloc(sizeof(char **), sh->count + 1);
	if (args == NULL)
		return (set_errno(ENOMEM), NULL);
	i = 0;
	while (i < sh->count)
	{
		args[i] = get_cmd_args(sh->in_pipes[i]);
		if (args[i] == NULL)
			return (free_d((void **)args[i]), set_errno(ENOMEM), NULL);
		i++;
	}
	return (set_flag(sh, 3), args);
}

pid_t	*create_pids(t_shell *sh)
{
	pid_t	*pid;

	pid = (pid_t *)ft_calloc(sizeof(pid_t), sh->count);
	if (pid == NULL)
		return (set_errno(ENOMEM), NULL);
	return (set_flag(sh, 5), pid);
}
