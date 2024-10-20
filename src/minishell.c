/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/19 19:29:41 by mfleury          ###   ########.fr       */
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
	int		n;
	int		p_fd[2];

	n = 0;
	while (pipes[n++] != NULL);
	args = (char ***)ft_calloc(sizeof(char **), n);
	pid = (pid_t *)ft_calloc(sizeof(pid_t), n);
	pid[0] = fork();
	i = 1;
	while (i < n)
	{
		if (pid[i - 1] != 0)
		{
			pid[i] = fork();
		}
		else if (pid[i - 1] == 0)
		{
			args[i] = get_cmd_args(pipes[i]);
			close(p_fd[1]);
			dup2(p_fd[0], 0);
			exec_cmd(args[i], envp);
			break;			
		}
		i++;
	}
	return (0);
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
		subshell(pipes, envp);
		wait(&wstatus);
		handle_cmd_return(wstatus, args);
		free_d(args);
		}
	}
	return (0);
}
