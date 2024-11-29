/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 16:08:32 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		**create_fpipes(t_pipe *p);
char	***create_args(t_pipe *p);
char	**create_cmd_names(t_pipe *p);
pid_t	*create_pids(t_pipe *p);
char	***create_redirs(t_pipe *p);
int		get_fdin_redir(t_pipe *p);
int		get_fdout_redir(t_pipe *p);
char	**identify_pipes(char *s_line, t_pipe **p);
int		count_pipes(char *line);

int	open_redir_fd(t_pipe *p)
{
	p->r_fd[INPUT] = get_fdin_redir(p);
	p->r_fd[OUTPUT] = get_fdout_redir(p);
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
	t_pipe *tmp;

	tmp = p;
	while (p != NULL)
	{
		if (p == tmp)
		{
			if (tmp == tmp->head)
				close(p->fd[READ_END]);
			else if (tmp->next == NULL)
				close(p->fd[WRITE_END]);
		}
		else
		{
			close(p->fd[READ_END]);
			close(p->fd[WRITE_END]);
		}
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
		dup2(p->fd[WRITE_END], STDOUT_FILENO);	// r_fd?
	if (p != p->head && p->next != NULL)
	{
		dup2(o->fd[READ_END], STDIN_FILENO); // r_fd?
		dup2(p->fd[WRITE_END], STDOUT_FILENO); // r_fd?
	}
	else if (p != p->head && p->next == NULL)
		dup2(o->fd[READ_END], STDIN_FILENO); //r_fd?
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


int	create_fork_pipe(t_pipe *p, char *envp[])
{
	t_pipe	*head;
	
	head = p->head;
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
	p = head;
	if (run_parent(p) == -1)
		return (free_pipe(p), -1);
	return (0);
}

/*int	clean_inputs(t_pipe *p)
{
	sh_trim_list_strings(p->in_pipes, "\"");
	sh_trim_list_strings(p->args[0], "\"");
	sh_trim_list_strings(p->redirs[0], "\"");
	
	
	return (0);
}
*/
t_pipe	*fill_pipes(t_pipe *p, char *line, int n)
{
	t_pipe	*tmp;
	int		i;

	i = 0;
	while (i < n)
	{
		line = sh_strtrim(&line, " ", 0);
		if (p == NULL)
			tmp = p_lstnew(&line);
		else
			tmp = p_lstadd_back(&p, &line);
		if (tmp == NULL)
			return (NULL);
		p = tmp->head;
		i++;
	}
	return (tmp->head);
}

int	subshell(t_shell *sh, char *envp[])
{
	int	errnum;
	t_pipe *p;

	errnum = 0;
	p = NULL;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(p, sh->s_line, sh->p_count);

	/*
	p->in_pipes = identify_pipes(sh->s_line, &p);
	p->fd = create_fpipes(p);
	p->pid = create_pids(p);
	//p->cmd = create_cmd_names(p);
	p->redirs = create_redirs(p);
	sh_trim_list_strings(p->in_pipes, " ");
	p->args = create_args(p);
	if (p->fd == NULL || p->args == NULL || p->pid == NULL)
		return (free_pipe(p), -1);
	clean_inputs(p);*/
	if (sh->p_count == 1)
	{
		if (ft_strncmp(p->args[0], "exit", 4) == 0)
			exit_minishell(sh, EXIT_SUCCESS);
		errnum = single_cmd(p, envp);
	}
	else if (create_fork_pipe(p, envp) == -1)
		return (free_pipe(p), -1);
	return (errnum);
}
