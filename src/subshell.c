/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:03:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		**create_fpipes(t_shell *sh);
char	***create_args(t_shell *sh);
pid_t	*create_pids(t_shell *sh);

static void	run_child(t_shell *sh, int i, char *envp[])
{
	int	j;
	int	wstatus;

	j = 0;
	while (j < sh->count)
	{
		if (j == i - 1)
			wstatus = dup2(sh->fd[j][0], 0);
		else
			wstatus = close(sh->fd[j][0]);
		if (wstatus == -1)
			exit(errno);
		if (j == i)
			wstatus = dup2(sh->fd[j][1], 1);
		else
			wstatus = close(sh->fd[j][1]);
		if (wstatus == -1)
			exit(errno);
		j++;
	}
	exit(exec_cmd(sh->args[i], envp));
}

static int	run_parent(t_shell *sh)
{
	int		j;
	char	c;

	j = 0;
	while (j < sh->count - 1)
	{
		if (close(sh->fd[j][0]) == -1)
			return (errno);
		if (close(sh->fd[j++][1]) == -1)
			return (errno);
	}
	if (close(sh->fd[sh->count - 1][1]) == -1)
		return (errno);
	while (read(sh->fd[sh->count - 1][0], &c, 1) > 0)
		if (write(1, &c, 1) == -1)
			return (errno);
	if (close(sh->fd[sh->count - 1][0]) == -1)
		return (errno);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
}

static int	create_fork(t_shell *sh, char *envp[])
{
	int	i;

	i = 0;
	while (i < sh->count)
	{
		sh->pid[i] = fork();
		if (sh->pid[i] == -1)
			return (errno);
		if (sh->pid[i] == 0)
			run_child(sh, i, envp);
		i++;
	}
	return (0);
}

void	sub_cmd_return(t_shell *sh, char *cmd, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (errno == 0)
			errno = WEXITSTATUS(wstatus);
		if (WEXITSTATUS(wstatus) == 255)
			set_flag(sh, 1);
		else if (WEXITSTATUS(wstatus) > 0 && WEXITSTATUS(wstatus) < 255)
			perror(cmd);
		errno = 0;
	}
}

int	subshell(t_shell *sh, char *envp[])
{
	int	i;
	int	wstatus;

	sh->fd = create_fpipes(sh);
	sh->args = create_args(sh);
	sh->pid = create_pids(sh);
	if (sh->fd == NULL || sh->args == NULL || sh->pid == NULL)
		return (free_sh(sh), errno);
	if (create_fork(sh, envp) == -1)
		return (free_sh(sh), errno);
	if (run_parent(sh) == -1)
		return (free_sh(sh), errno);
	i = 0;
	while (i < sh->count)
	{
		waitpid(sh->pid[i], &wstatus, 0);
		sub_cmd_return(sh, sh->args[i++][0], wstatus);
	}
	return (free_sh(sh), 0);
}
