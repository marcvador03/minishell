/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/24 20:13:10 by mfleury          ###   ########.fr       */
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

int	single_cmd(t_pipe *p, char *envp[])
{
	int	wstatus;
		
	open_redir_fd(p, 0);
	wstatus = exec_cmd(p->args[0][0], p->args[0], p->count, envp);
	close_redir_fd(p);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (wstatus);
}

int	close_pipes(t_pipe *p, int n)
{
	int	i;

	i = 0;
	while (i < p->count - 1)
	{
		if (i == n)
		{
			if (n == 0)
				close(p->fd[i][READ_END]);
			else if (n == p->count - 1)
				close(p->fd[i][WRITE_END]);
		}
		else
		{
			close(p->fd[i][READ_END]);
			close(p->fd[i][WRITE_END]);
		}
		i++;
	}
	return (0);
}

int	run_child(t_pipe *p, int i, char *envp[])
{
	int wstatus;
	
	if (i == 0)
		dup2(p->fd[i][WRITE_END], STDOUT_FILENO);	// r_fd?
	if (i != 0 && i != p->count - 1)
	{
		dup2(p->fd[i - 1][READ_END], STDIN_FILENO); // r_fd?
		dup2(p->fd[i][WRITE_END], STDOUT_FILENO); // r_fd?
	}
	else if (i != 0 && i == p->count - 1)
		dup2(p->fd[i - 1][READ_END], STDIN_FILENO); //r_fd?
	close_pipes(p, i);
	open_redir_fd(p, i);
	wstatus = exec_cmd(p->args[i][0], p->args[i], p->count, envp);
	close_redir_fd(p);
	return (wstatus);
}

static int	run_parent(t_shell *sh, t_pipe *p)
{
	int		i;
	int	wstatus;
	
	i = 0;
	while (i < p->count - 1)
	{
		close(p->fd[i][READ_END]);
		close(p->fd[i][WRITE_END]);
		i++;
	}
	i = 0;
	while (i < p->count)
	{
		waitpid(p->pid[i], &wstatus, 0);
		sub_cmd_return(sh, p->args[i++][0], wstatus, NULL);
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
}


int	create_fork_pipe(t_shell *sh, t_pipe *p, char *envp[])
{
	int	i;

	i = 0;
	while (i < p->count)
	{
		p->pid[i] = fork();
		if (p->pid[i] == -1)
			return (-1);
		if (p->pid[i] == 0)
			run_child(p, i, envp);
		i++;
	}
	i = 0;
	if (run_parent(sh, p) == -1)
		return (free_pipe(p), -1);
	return (0);
}

int	subshell(t_shell *sh, t_pipe *p, char *envp[])
{
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
		errnum = single_cmd(p, envp);
	else if (create_fork_pipe(sh, p, envp) == -1)
		return (free_pipe(p), -1);
	return (errnum);
}
