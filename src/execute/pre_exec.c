/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:58:50 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 19:49:33 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_fd(t_pipe *p);
int	close_redir_fd(t_pipe *p);
int	close_pipes(t_pipe *p);
int	exec_cmd(char *cmd, char **args, int pcount, char *envp[]);

static int	run_child(t_pipe *p, char *envp[])
{
	int		wstatus;
	t_pipe	*o;

	o = p->prev;
	if (p == p->head)
		dup2(p->fd[WRITE_END], STDOUT_FILENO);
	if (p != p->head && p->next != NULL)
	{
		dup2(o->fd[READ_END], STDIN_FILENO);
		dup2(p->fd[WRITE_END], STDOUT_FILENO);
	}
	else if (p != p->head && p->next == NULL)
		dup2(o->fd[READ_END], STDIN_FILENO);
	close_pipes(p);
	open_redir_fd(p);
	wstatus = exec_cmd(p->args[0], p->args, 0, envp);
	close_redir_fd(p);
	exit (wstatus);
}

static int	run_parent(t_pipe *p)
{
	int		wstatus;
	t_pipe	*head;

	head = p->head;
	p = head;
	while (p != NULL)
	{
		close(p->fd[READ_END]);
		close(p->fd[WRITE_END]);
		p = p->next;
	}
	p = head;
	while (p != NULL)
	{
		waitpid(p->pid, &wstatus, 0);
		p = p->next;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	return (0);
}

static int	create_pipes(t_pipe *p)
{
	t_pipe	*head;

	head = p->head;
	p = head;
	while (p != NULL)
	{
		if (pipe(p->fd) == -1)
			return (-1);
		p = p->next;
	}
	return (0);
}

int	single_cmd(t_pipe *p, char *envp[])
{
	int	wstatus;

	open_redir_fd(p);
	wstatus = exec_cmd(p->args[0], p->args, 1, envp);
	close_redir_fd(p);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (wstatus);
}

int	multiple_cmd(t_pipe *p, char *envp[])
{
	t_pipe	*head;

	init_signal(0);
	head = p->head;
	p = head;
	if (create_pipes(p) == -1)
		return (-1);
	p = head;
	while (p != NULL)
	{
		p->pid = fork();
		if (p->pid == -1)
			return (-1);
		if (p->pid == 0)
			run_child(p, envp);
		p = p->next;
	}
	if (run_parent(head) == -1)
		return (free_pipe(head), -1);
	return (0);
}
