/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/21 19:40:32 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		**create_fpipes(t_pipe *p);
char	***create_args(t_pipe *p);
char	**create_cmd_names(t_pipe *p);
pid_t	*create_pids(t_pipe *p);
char	***create_redirs(t_pipe *p);
int		get_fdin_redir(t_pipe *p, int n);
int		get_fdout_redir(t_pipe *p, int n);



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

int	open_redir_fd(t_pipe *p, int n)
{
	p->r_fd[INPUT] = get_fdin_redir(p, n);
	p->r_fd[OUTPUT] = get_fdout_redir(p, n);
	if (p->r_fd[INPUT] > 2)
	{
		p->r_fd[T_INPUT] = dup(STDIN_FILENO);
		dup2(p->r_fd[INPUT], STDIN_FILENO);
	}
	if (p->r_fd[OUTPUT] > 2)
	{
		p->r_fd[T_OUTPUT] = dup(STDOUT_FILENO);
		dup2(p->r_fd[OUTPUT], STDOUT_FILENO);
	}
	return (0);
}

int	close_redir_fd(t_pipe *p)
{
	if (p->r_fd[INPUT] > 2)
	{
		close(p->r_fd[INPUT]);
		dup2(p->r_fd[T_INPUT], STDIN_FILENO);
		close(p->r_fd[T_INPUT]);
	}
	if (p->r_fd[OUTPUT] > 2)
	{
		close(p->r_fd[OUTPUT]);
		dup2(p->r_fd[T_OUTPUT], STDOUT_FILENO);
		close(p->r_fd[T_OUTPUT]);
	}
	return (0);
}

int	single_cmd(t_shell *sh, t_pipe *p, char *envp[])
{
	int	wstatus;
		
	open_redir_fd(p, 0);
	wstatus = exec_cmd(sh, p->args[0][0], p->args[0], envp);
	close_redir_fd(p);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (wstatus);
}

int	run_child(t_shell *sh, t_pipe *p, int i, char *envp[])
{
	int		j;
	int		err;

	j = 0;
	err = 0;
	while (j < p->count)
	{
		/*if (j == i - 1)
			err = dup2(p->fd[j][0], STDOUT_FILENO);
		else*/
			err = close(p->fd[j][READ_END]);
		if (err == -1)
			exit(err);
		if (j == i)
			err = dup2(p->fd[j][WRITE_END], STDIN_FILENO);
		else
			err = close(p->fd[j][WRITE_END]);
		if (err == -1)
			exit(err);
		j++;
	}
	exit(exec_cmd(sh, p->args[i][0], p->args[i], envp));
}

int	create_fork_pipe(t_shell *sh, t_pipe *p, char *envp[])
{
	int	i;

	i = 0;
	while (i < p->count)
	{
		//open_redir_fd(p, i);
		p->pid[i] = fork();
		if (p->pid[i] == -1)
			return (-1);
		if (p->pid[i] == 0)
			run_child(sh, p, i, envp);
		//close_redir_fd(p);
		i++;
	}
	return (0);
}

static int	run_parent(t_pipe *p)
{
	int		j;
	char	c;

	j = 0;
	while (j < p->count - 1)
	{
		if (close(p->fd[j][READ_END]) == -1)
			return (-1);
		if (close(p->fd[j++][WRITE_END]) == -1)
			return (-1);
	}
	if (close(p->fd[p->count - 1][WRITE_END]) == -1)
		return (-1);
	while (read(p->fd[p->count - 1][READ_END], &c, 1) > 0)
		if (write(STDOUT_FILENO, &c, 1) == -1)
			return (-1);
	if (close(p->fd[p->count - 1][READ_END]) == -1)
		return (-1);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
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
	if (p->count == 1)
		errnum = single_cmd(sh, p, envp);
	 else if (create_fork_pipe(sh, p, envp) == -1)
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
