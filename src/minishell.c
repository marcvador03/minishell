/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/17 15:16:56 by mfleury          ###   ########.fr       */
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

int	main(int argc, char *argv[], char *envp[])
{
	char	**args;
	pid_t	pid[2];
	int		wstatus;
	
	if (argc > 1 || argv == NULL)
		return (1);	
	wstatus = 0;
	while (1)
	{
		args = get_cmd_args();
		if (args != NULL && args[0] != NULL)
		{
			pid[0] = fork();
			if (pid[0] == -1)
				return(errno);
			if (pid[0] == 0)
				exit(exec_cmd(args, envp));
			else
			{
				wait(&wstatus);
				handle_cmd_return(wstatus, args);
			}
		}
		free_d(args);
	}
	return (0);
}
