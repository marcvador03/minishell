/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/18 21:02:25 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		**create_fpipes(t_pipe *p);
char	***create_args(t_pipe *p);
char	**create_cmd_names(t_pipe *p);
pid_t	*create_pids(t_pipe *p);
char	***create_redirs(t_pipe *p);
int		get_fdin_redir(t_pipe *p, int n);
int		get_fdout_redir(t_pipe *p, int n);

static int	run_child(t_pipe *p, int i, char *envp[])
{
	int		j;
	int		err;

	j = 0;
	err = 0;
	while (j < p->count)
	{
		if (j == i - 1)
			err = dup2(p->fd[j][0], STDOUT_FILENO);
		else
			err = close(p->fd[j][0]);
		if (err == -1)
			exit(err);
		if (j == i)
			err = dup2(p->fd[j][1], STDIN_FILENO);
		else
			err = close(p->fd[j][1]);
		if (err == -1)
			exit(err);
		j++;
	}
	exit(exec_cmd(p->args[i][0], p->args[i], envp));
}

static int	run_parent(t_pipe *p)
{
	int		j;
	char	c;

	j = 0;
	while (j < p->count - 1)
	{
		if (close(p->fd[j][0]) == -1)
			return (-1);
		if (close(p->fd[j++][1]) == -1)
			return (-1);
	}
	if (close(p->fd[p->count - 1][1]) == -1)
		return (-1);
	while (read(p->fd[p->count - 1][0], &c, 1) > 0)
		if (write(STDOUT_FILENO, &c, 1) == -1)
			return (-1);
	if (close(p->fd[p->count - 1][0]) == -1)
		return (-1);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
}

static int	create_fork_pipe(t_pipe *p, char *envp[])
{
	int	i;
	int	fd_in;
	int	fd_out;

	i = 0;
	while (i < p->count)
	{
		fd_in = get_fdin_redir(p, i);
		fd_out = get_fdout_redir(p, i);
		dup2(STDIN_FILENO, fd_in);
		dup2(STDOUT_FILENO, fd_out);
		p->pid[i] = fork();
		if (p->pid[i] == -1)
			return (-1);
		if (p->pid[i] == 0)
			run_child(p, i, envp);
		i++;
	}
	return (0);
}

void	sub_cmd_return(t_shell *sh, char *cmd, int wstatus, int *errnum)
{
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == 255 && sh->pipes->count == 1)
			exit_minishell(sh, EXIT_SUCCESS);
		if (WEXITSTATUS(wstatus) > 0 && WEXITSTATUS(wstatus) < 255)
		{
			if (errno != 0)
				perror(cmd);
			else
				printf("%s: %s\n", cmd, strerror(WEXITSTATUS(wstatus)));
			*errnum = 1;
		}
	}
}

int	subshell(t_shell *sh, t_pipe *p, char *envp[])
{
	int	i;
	int	wstatus;
	int	errnum;

	errnum = 0;
	p->fd = create_fpipes(p);
	p->pid = create_pids(p);
	//p->cmd = create_cmd_names(p);
	p->redirs = create_redirs(p);
	clean_spaces(p->in_pipes);
	p->args = create_args(p);
	if (p->fd == NULL || p->args == NULL || p->pid == NULL)
		return (free_pipe(p), -1);
	if (create_fork_pipe(p, envp) == -1)
		return (free_pipe(p), -1);
	if (run_parent(p) == -1)
		return (free_pipe(p), -1);
	i = 0;
	while (i < p->count)
	{
		waitpid(p->pid[i], &wstatus, 0);
		sub_cmd_return(sh, p->args[i++][0], wstatus, &errnum);
	}
	return (errnum);
}
