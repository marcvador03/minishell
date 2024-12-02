/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 12:09:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		**create_fpipes(t_pipe *p);
char	***create_args(t_pipe *p);
char	**create_cmd_names(t_pipe *p);
pid_t	*create_pids(t_pipe *p);
char	***create_redirs(t_pipe *p);
char	**identify_pipes(char *s_line, t_pipe **p);
int		count_pipes(char *line);

int	open_redir_fd(t_pipe *p)
{
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
		
	open_redir_fd(p);
	wstatus = exec_cmd(p->args[0], p->args, 1, envp);
	close_redir_fd(p);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (wstatus);
}

int	close_pipes(t_pipe *p)
{
	//t_pipe *tmp;

	//tmp = p;
	p = p->head;
	while (p != NULL)
	{
		/*if (p == tmp)
		{
			if (tmp == tmp->head)
				close(p->fd[READ_END]);
			else if (tmp != tmp->head && tmp->next == NULL)
				close(p->fd[WRITE_END]);
		}
		else
		{*/
			close(p->fd[READ_END]);
			close(p->fd[WRITE_END]);
		//}
		p = p->next;
	}
	return (0);
}

int	run_child(t_pipe *p, char *envp[])
{
	int wstatus;
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
	int	wstatus;
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

int	create_pipes(t_pipe *p)
{
	t_pipe	*head;
	
	head = p->head;
	p = head;
	while (p!= NULL)
	{
		if (pipe(p->fd) == -1)
			return (-1);
		p = p->next;
	}
	return (0);
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

t_pipe	*fill_pipes(t_pipe *p, char *line, int n)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	t_line = ft_strdup(line);
	while (i < n)
	{
		t_line = sh_strtrim(t_line, " ", 0); // leak
		if (p == NULL)
			tmp = p_lstnew(&t_line);
		else
			tmp = p_lstadd_back(&p, &t_line);
		if (tmp == NULL)
			return (free_s((void *)t_line), NULL);
		p = tmp->head;
		i++;
	}
	return (free_s((void *)t_line), tmp->head);
}

int	subshell(t_shell *sh, char *envp[])
{
	int	errnum;
	t_pipe *p;

	errnum = 0;
	p = NULL;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(p, sh->s_line, sh->p_count);
	sh->pipes = p->head;
	if (sh->p_count == 1)
	{
		if (ft_strncmp(p->args[0], "exit", 4) == 0)
			exit_minishell(sh, EXIT_SUCCESS);
		errnum = single_cmd(p, envp);
	}
	else if (multiple_cmd(p, envp) == -1)
		return (free_pipe(sh->pipes), -1);
	return (free_pipe(sh->pipes), errnum);
}
