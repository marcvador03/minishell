/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/23 22:16:03 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int **create_fpipes(int n)
{
	int	i;
	int	**p_fd;
	
	p_fd = (int **)ft_calloc(sizeof(int *), n);
	if (p_fd == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		p_fd[i] = (int *)ft_calloc(sizeof(int), 2);
		if (p_fd[i] == NULL)
			return (NULL);
		pipe(p_fd[i++]);
	}
	return (p_fd);
}

static char	***create_args(t_shell *sh)
{
	char	***args;
	int		i;

	args = (char ***)ft_calloc(sizeof(char **), sh->count + 1);
	if (args == NULL)
		return (NULL);
	i = 0;
	while (i < sh->count)
	{
		args[i] = get_cmd_args(sh->in_pipes[i]);
		i++;
	}
	return (args);
}

static int run_child(t_shell *sh, int i, char *envp[]) 
{
	int 	j;
	
	j = 0;
	while (j < sh->count)
	{
		if (j == i - 1)
			dup2(sh->fd[j][0], 0);
		else
			close(sh->fd[j][0]);	
		if (j == i)
			dup2(sh->fd[j][1], 1);
		else
			close(sh->fd[j][1]);	
		j++;
	}
	exit(exec_cmd(sh->args[i], envp));
}

static void	create_fork(t_shell *sh, char *envp[])
{
	int	i;

	i = 0;
	while (i < sh->count)
	{
			sh->pid[i] = fork();
		if (sh->pid[i] == 0)
			sh->wstatus = run_child(sh, i, envp);
		i++;
	}
}

static void	run_parent(t_shell *sh)
{
	int		j;
	char	c;
		
	j = 0;
	while (j < sh->count - 1)
	{
		close(sh->fd[j][0]);	
		close(sh->fd[j++][1]);	
	}
	close(sh->fd[sh->count - 1][1]);
	while (read(sh->fd[sh->count - 1][0], &c, 1) > 0)
		write(1, &c, 1);
	close(sh->fd[sh->count - 1][0]);
	rl_replace_line("", 0);
	rl_on_new_line();
}	  

int	subshell(t_shell *sh, char *envp[])
{
	int		i;

	sh->fd = create_fpipes(sh->count);
	sh->pid = (pid_t *)ft_calloc(sizeof(pid_t), sh->count);
	sh->args = create_args(sh);
	if (sh->fd  == NULL || sh->args == NULL || sh->pid == NULL)
		return (free_d((void **)sh->fd), free(sh->args), free(sh->pid), ENOMEM);
	create_fork(sh, envp);
	run_parent(sh);
	i = 0;
	while (i < sh->count)
	{
		waitpid(sh->pid[i], &(sh->wstatus), 0);
		i++;
	}
	return (sh->wstatus);
}
