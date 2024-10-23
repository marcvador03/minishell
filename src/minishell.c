/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/23 22:14:07 by mfleury          ###   ########.fr       */
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


void	handle_cmd_return(t_shell *sh)
{
	errno = WEXITSTATUS(sh->wstatus);
	if (WIFEXITED(sh->wstatus) && WEXITSTATUS(sh->wstatus) == 99)
		exit_minishell(EXIT_SUCCESS, 0/*, args*/);
	else if (WIFEXITED(sh->wstatus) && WEXITSTATUS(sh->wstatus) != 0)
		perror("cmd: ");
}



int	main(int argc, char *argv[], char *envp[])
{
	t_shell	sh;	
	
	if (argc > 1 || argv == NULL)
		return (1);	
	sh.wstatus = 0;
	sh.count = 0;
	while (1)
	{
		sh.in_pipes = get_input();
		while (sh.in_pipes[sh.count++] != NULL);
		sh.count--;
		if (sh.in_pipes == NULL)
			perror("minishell");
		if (sh.in_pipes != NULL && sh.in_pipes[1] == NULL)
		{
			if (ft_strncmp(sh.in_pipes[0], "", ft_strlen(sh.in_pipes[0] + 1) + 1) != 0)
				sh.wstatus = subshell(&sh, envp);
		}
		else
			sh.wstatus = subshell(&sh, envp);
		handle_cmd_return(&sh);
	}
	return (0);
}
