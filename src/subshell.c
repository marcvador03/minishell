/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/22 16:52:08 by mfleury          ###   ########.fr       */
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

static void	child_man_pipes(int n, int i, char **in_pipes, int **p_fd)
{
	int	j;

	j = 0;
	while (j < n)
	{
		if (j == i - 1)
			dup2(p_fd[j][0], 0);
		else
			close(p_fd[j][0]);	
		if (j == i)
			dup2(p_fd[j][1], 1);
		else
			close(p_fd[j][1]);	
		j++;
	}
}


static char	***create_forks(int n, char **in_pipes, pid_t *pid)
{
	char	***args;
	int		i;

	args = (char ***)ft_calloc(sizeof(char **), n + 1);
	if (args == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
			args[i] = get_cmd_args(in_pipes[i]);
			pid[i] = fork();
		if (pid[i] == 0)
			break;
		i++;
	}
	return (args);
}	

int	subshell(int n, char **in_pipes, char *envp[])
{
	pid_t	*pid;
	char	***args;
	int		i;
	int		j;
	int		**p_fd;
	int		wstatus;
	char	c;

	wstatus = 0;
	p_fd = create_fpipes(n);
	pid = (pid_t *)ft_calloc(sizeof(pid_t), n);
	args = create_forks(n, in_pipes, pid);
	if (p_fd  == NULL || args == NULL || pid == NULL)
		return (free_d((void **)p_fd), free(args), free(pid), ENOMEM);
	if (pid[--i] == 0)
	{
		j = 0;
		while (j < n)
		{
			if (j == i - 1)
				dup2(p_fd[j][0], 0);
			else
				close(p_fd[j][0]);	
			if (j == i)
				dup2(p_fd[j][1], 1);
			else
				close(p_fd[j][1]);	
			j++;
		}
		wstatus = exec_cmd(args[i], envp);
		exit (wstatus);
	}
	j = 0;
	while (j < n - 1)
	{
		close(p_fd[j][0]);	
		close(p_fd[j++][1]);	
	}
	//dup2(p_fd[n - 1][0], 0);
	close(p_fd[n - 1][1]);
	while (read(p_fd[n - 1][0], &c, 1) > 0)
		write(1, &c, 1);
	close(p_fd[n - 1][0]);
	rl_replace_line("", 0);
	rl_on_new_line();
	//rl_redisplay();
	i = 0;
	while (i < n)
	{
		waitpid(pid[i], &wstatus, 0);
		//handle_cmd_return(wstatus);
		i++;
	}
	return (wstatus);
}
