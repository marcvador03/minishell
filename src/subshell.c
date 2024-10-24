/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/24 16:25:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int **create_fpipes(t_shell *sh)
{
	int	i;
	int	**fd;
	
	fd = (int **)ft_calloc(sizeof(int *), sh->count + 1);
	if (fd == NULL)
		return (NULL);
	i = 0;
	while (i < sh->count)
	{
		fd[i] = (int *)ft_calloc(sizeof(int), 2);
		if (fd[i] == NULL)
			return (free_d((void **)fd), NULL);
		pipe(fd[i++]);
	}
	fd[i] = NULL;
	return (set_flag(sh, 3), fd);
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
	return (set_flag(sh, 2), args);
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

static void	create_pids(t_shell *sh)
{
	sh->pid = (pid_t *)ft_calloc(sizeof(pid_t), sh->count);
	if (sh->pid != NULL)
		set_flag(sh, 4);
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

	sh->fd = create_fpipes(sh);
	sh->args = create_args(sh);
	create_pids(sh);
	if (sh->fd  == NULL || sh->args == NULL || sh->pid == NULL)
		return (free_sh(sh), ENOMEM);
	create_fork(sh, envp);
	run_parent(sh);
	i = 0;
	while (i < sh->count)
	{
		waitpid(sh->pid[i], &(sh->wstatus), 0);
		i++;
	}
	return (free_sh(sh), sh->wstatus);
}
