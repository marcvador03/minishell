/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_exec_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:58:50 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 13:08:10 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes_child(t_pipe *p);
int	exec_cmd(char *cmd, char **args, t_pipe *p, t_env *env);

static int	run_child(t_pipe *p, t_env *env)
{
	int		wstatus;

	wstatus = 0;
	if (create_pipes_child(p) == -1)
		return (flush_errors("", 1, 0));
	close_pipes(p);
	if (p->p_line == NULL)
		exit (wstatus);
	if (open_redir_fd(p->r, &p->p_status, p->args[0]) == -1)
	{
		close_redir_fd_single(p->r, &p->p_status, p->args[0]);
		close_redir_fd_pipe(p);
		return (p->p_status);
	}
	if (p->args[0] != NULL)
		wstatus = exec_cmd(p->args[0], p->args, p, env);
	close_redir_fd_single(p->r, &p->p_status, p->args[0]);
	close_redir_fd_pipe(p);
	exit (wstatus);
}

static int	run_parent(t_pipe *p)
{
	int		wstatus;
	t_pipe	*head;
	pid_t	pid;

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
		pid = waitpid(0, &wstatus, 0);
		main_cmd_return(p, wstatus, pid);
		if (p->p_status == 125 || p->p_status == 124)
			p->p_status = p->p_status + 2;
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

int	single_cmd(t_pipe *p, t_env *env)
{
	if (p->p_line == NULL)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		return (p->p_status);
	}
	if (open_redir_fd(p->r, &p->p_status, p->args[0]) == -1)
	{
		close_redir_fd_single(p->r, &p->p_status, p->args[0]);
		close_redir_fd_pipe(p);
		return (p->p_status);
	}
	if (p->args[0] != NULL)
		exec_cmd(p->args[0], p->args, p, env);
	if (p->p_status == 125 || p->p_status == 124)
		p->p_status = p->p_status + 2;
	close_redir_fd_single(p->r, &p->p_status, p->args[0]);
	close_redir_fd_pipe(p);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (p->p_status);
}

int	multiple_cmd(t_pipe *p, t_env *env)
{
	t_pipe	*head;

	init_signal(0, 0);
	head = p->head;
	p = head;
	if (create_pipes(p) == -1)
		return (flush_errors("", -1, 0));
	p = head;
	while (p != NULL)
	{
		p->pid = fork();
		if (p->pid == -1)
			return (-1);
		if (p->pid == 0)
			run_child(p, env);
		p = p->next;
	}
	if (run_parent(head) == -1)
		return (free_pipe(head), -1);
	while (head->next != NULL)
		head = head->next;
	return (head->p_status);
}
