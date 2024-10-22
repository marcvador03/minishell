/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/22 16:36:47 by mfleury          ###   ########.fr       */
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



int	main(int argc, char *argv[], char *envp[])
{
	
	char	**in_pipes;
	int		wstatus;
	int		n;
	
	if (argc > 1 || argv == NULL)
		return (1);	
	wstatus = 0;
	n = 0;
	while (1)
	{
		in_pipes = get_input();
		while (in_pipes[n++] != NULL);
		if (in_pipes == NULL)
			perror("minishell");
		if (in_pipes != NULL && in_pipes[1] == NULL)
		{
			if (ft_strncmp(in_pipes[0], "", ft_strlen(in_pipes[0] + 1) + 1) != 0)
				wstatus = subshell(--n, in_pipes, envp);
		}
		else
			wstatus = subshell(--n, in_pipes, envp);
		handle_cmd_return(wstatus);
	}
	return (0);
}
