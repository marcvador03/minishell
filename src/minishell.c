/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/22 01:09:28 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	exit_minishell(int status, int err, char **args)
{
	free_d(args);
	if (err != 0)
		errno = err;
	if (errno == 99)
		printf("Minishell exited with success\n");
	else
		perror("Minishell exit with error:\n");
	exit(status);
}


void	handle_cmd_return(int wstatus, char **args)
{
	errno = WEXITSTATUS(wstatus);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 99)
		exit_minishell(EXIT_SUCCESS, 0, args);
	else if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) != 0)
		perror(args[0]);
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

	n = 0;
	while (pipes[n++] != NULL);
	args = (char ***)ft_calloc(sizeof(char **), n);
	pid = (pid_t *)ft_calloc(sizeof(pid_t), n - 1);
	p_fd = (int **)ft_calloc(sizeof(int *), n - 1);
	i = 0;
	wstatus = 0;
	while (i < n - 1)
	{
		/*if (i == 0 || (pid[i - 1] != 0 && i > 0))
		{*/
			p_fd[i] = (int *)ft_calloc(sizeof(int), 2);
			pipe(p_fd[i]);
			args[i] = get_cmd_args(pipes[i]);
			pid[i] = fork();
		//}
		if (pid[i++] == 0)
			break;
	}
	if (pid[--i] == 0)
	{
		printf("Pid %d\n", i);
		j = 0;
		while (j < i - 1)
		{
			close(p_fd[j][0]);	
			close(p_fd[j++][1]);	
		}
		close(p_fd[i - 1][1]);	
		if (i != 0)
			dup2(p_fd[i - 1][0], 0);
		else
			close(p_fd[i - 1][0]);	
		dup2(p_fd[i][1], 1);
		wstatus = exec_cmd(args[i], envp);
		exit (wstatus);
	}
	i = 0;
	while (i < n - 1)
	{
		waitpid(pid[i], &wstatus, 0);
		handle_cmd_return(wstatus, args[i]);
		i++;
	}
	j = 0;
	while (j < n - 2)
	{
		close(p_fd[j][0]);	
		close(p_fd[j++][1]);	
	}
	dup2(p_fd[n - 1][0], 0);
	close(p_fd[n - 1][1]);
	return (0);
}


int	main(int argc, char *argv[], char *envp[])
{
	
	char	**pipes;
	//int		wstatus;
	
	if (argc > 1 || argv == NULL)
		return (1);	
	//wstatus = 0;
	while (1)
	{
		pipes = get_input();
		if (pipes == NULL)
			perror("minishell");
		if (pipes[1] == NULL)
		{
			if (ft_strncmp(pipes[0], "", ft_strlen(pipes[0] + 1) + 1) != 0)
				subshell(pipes, envp);
		}
		else
			subshell(pipes, envp);
	}
	return (0);
}
