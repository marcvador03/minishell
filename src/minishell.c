/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/22 14:29:11 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	exit_minishell(int status, int err/*, char **args*/)
{
	//free_d((void **)args);
	if (err != 0)
		errno = err;
	if (errno == 99)
		printf("Minishell exited with success\n");
	else
		perror("Minishell exit with error:\n");
	exit(status);
}


void	handle_cmd_return(int wstatus/*, char **args*/)
{
	errno = WEXITSTATUS(wstatus);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 99)
		exit_minishell(EXIT_SUCCESS, 0/*, args*/);
	else if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) != 0)
		perror("cmd: ");
}

int	subshell(char **pipes, char *envp[])
{
	pid_t	*pid;
	char	***args;
	int		i;
	int		j;
	int		n;
	int		**p_fd;
	int		wstatus;
	char	c;

	n = 0;
	wstatus = 0;
	while (pipes[n++] != NULL);
	n--;
	args = (char ***)ft_calloc(sizeof(char **), n + 1);
	if (args == NULL)
		return (ENOMEM);
	pid = (pid_t *)ft_calloc(sizeof(pid_t), n);
	if (pid == NULL)
		return (free(args), ENOMEM);
	p_fd = (int **)ft_calloc(sizeof(int *), n);
	if (p_fd == NULL)
		return (free(pid), free(args), ENOMEM);
	i = 0;
	while (i < n)
	{
		p_fd[i] = (int *)ft_calloc(sizeof(int), 2);
		if (p_fd[i] == NULL)
			return (free_d((void **)p_fd), free(args), ENOMEM);
		pipe(p_fd[i++]);
	}
	i = 0;
	while (i < n)
	{
		/*if (i == 0 || (pid[i - 1] != 0 && i > 0))
		{*/
			args[i] = get_cmd_args(pipes[i]);
			pid[i] = fork();
		//}
		if (pid[i++] == 0)
			break;
	}
	if (pid[--i] == 0)
	{
		//printf("Pipe %d\n", i);
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
	dup2(p_fd[n - 1][0], 0);
	close(p_fd[n - 1][1]);
	close(p_fd[n - 1][0]);
	while (read(0, &c, 1) > 0)
		write(1, &c, 1);
	i = 0;
	while (i < n)
	{
		waitpid(pid[i], &wstatus, 0);
		//handle_cmd_return(wstatus);
		i++;
	}
	return (wstatus);
}


int	main(int argc, char *argv[], char *envp[])
{
	
	char	**pipes;
	int		wstatus;
	
	if (argc > 1 || argv == NULL)
		return (1);	
	wstatus = 0;
	while (1)
	{
		pipes = get_input();
		if (pipes == NULL)
			perror("minishell");
		if (pipes[1] == NULL)
		{
			if (ft_strncmp(pipes[0], "", ft_strlen(pipes[0] + 1) + 1) != 0)
				wstatus = subshell(pipes, envp);
		}
		else
			wstatus = subshell(pipes, envp);
		handle_cmd_return(wstatus);
	}
	return (0);
}
