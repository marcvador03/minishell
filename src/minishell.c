/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/18 14:43:54 by mfleury          ###   ########.fr       */
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

int	create_subshell(char **pipes, char *envp[])
{
	pid_t	*pid;
	char	**args;
	int		i;
	int		n;

	n = 0;
	while (pipes[n++] != NULL)
	pid = (pid_t *)ft_calloc(n, sizeof(pid_t));
	if (pid == NULL)
		return(ENOMEM);
	i = 0;
	pid[0] = fork();
	if (pid[0] == -1)
		exit(errno);
	if (pid[0] == 0)	
	{
		while (i < n)
		{
			if (pid[i++] == 0)
			{
				pid[i] = fork();
				if (pid[i] == -1)
					exit(errno);
			}
			if (pid[i - 1] == 0)
				break;
		}
		args = get_cmd_args(pipes[0]);
		if (args == NULL)
			exit(errno);
		exit(exec_cmd(args, envp));
	}
	return (0);
}


int	main(int argc, char *argv[], char *envp[])
{
	
	char	**pipes;
	char	**sub_args;
	pid_t	pid[2];
	int		wstatus;
	
	if (argc > 1 || argv == NULL)
		return (1);	
	wstatus = 0;
	while (1)
	{
		pipes = get_input();
		if (pipes == NULL)
			perror("minishell");
		wait(&wstatus);
		handle_cmd_return(wstatus, args);
		free_d(args);
		}
	}
	return (0);
}
