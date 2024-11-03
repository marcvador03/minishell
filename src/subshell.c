/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/02 23:19:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		**create_fpipes(t_pipe *p);
char	***create_args(t_pipe *p);
pid_t	*create_pids(t_pipe *p);

static void	run_child(t_pipe *p, int i, char *envp[])
{
	int	j;
	int	wstatus;

	j = 0;
	while (j < p->count)
	{
		if (j == i - 1)
			wstatus = dup2(p->fd[j][0], 0);
		else
			wstatus = close(p->fd[j][0]);
		if (wstatus == -1)
			exit(errno);
		if (j == i)
			wstatus = dup2(p->fd[j][1], 1);
		else
			wstatus = close(p->fd[j][1]);
		if (wstatus == -1)
			exit(errno);
		j++;
	}
	exit(exec_cmd(p->args[i], envp));
}

static int	run_parent(t_pipe *p)
{
	int		j;
	char	c;

	j = 0;
	while (j < p->count - 1)
	{
		if (close(p->fd[j][0]) == -1)
			return (errno);
		if (close(p->fd[j++][1]) == -1)
			return (errno);
	}
	if (close(p->fd[p->count - 1][1]) == -1)
		return (errno);
	while (read(p->fd[p->count - 1][0], &c, 1) > 0)
		if (write(1, &c, 1) == -1)
			return (errno);
	if (close(p->fd[p->count - 1][0]) == -1)
		return (errno);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
}

static int	create_fork_pipe(t_pipe *p, char *envp[])
{
	int	i;

	i = 0;
	while (i < p->count)
	{
		p->pid[i] = fork();
		if (p->pid[i] == -1)
			return (errno);
		if (p->pid[i] == 0)
			run_child(p, i, envp);
		i++;
	}
	return (0);
}

void	sub_cmd_return(char *cmd, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (errno == 0)
			errno = WEXITSTATUS(wstatus);
		/*if (WEXITSTATUS(wstatus) == 255)
			exit_minishell(p, EXIT_SUCCESS, 0);*/
		if (WEXITSTATUS(wstatus) > 0 && WEXITSTATUS(wstatus) < 255)
			perror(cmd);
		errno = 0;
	}
}

int	subshell(t_pipe *p, char *envp[])
{
	int	i;
	int	wstatus;

	p->fd = create_fpipes(p);
	p->args = create_args(p);
	p->pid = create_pids(p);
	if (p->fd == NULL || p->args == NULL || p->pid == NULL)
		return (free_pipe(p), errno);
	if (create_fork_pipe(p, envp) == -1)
		return (free_pipe(p), errno);
	if (run_parent(p) == -1)
		return (free_pipe(p), errno);
	i = 0;
	while (i < p->count)
	{
		waitpid(p->pid[i], &wstatus, 0);
		sub_cmd_return(p->args[i++][0], wstatus);
	}
	return (free_pipe(p), WEXITSTATUS(wstatus));
}
