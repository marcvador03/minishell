/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/07 14:34:13 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main_cmd_return(t_shell *sh, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		if (sh->pipes->mem_flag & (1 << 7))
			exit_minishell(sh, EXIT_SUCCESS);
		if (WEXITSTATUS(wstatus) > 0 && WEXITSTATUS(wstatus) < 255)
			perror("minishell: ");
	}
	return (0);
}

void	exit_minishell(t_shell *sh, int status)
{
	if (errno == 0 || errno == 255)
		printf("Minishell exited with success\n");
	else
		perror("Minishell exited with error:\n");
	free_sh(sh);
	exit(status);
}

int	main(int argc, char *argv[], char *envp[])
{
	if (argc > 1 || argv == NULL)
		return (1);
	while (1)
		if (start_shell(envp) != 0)
			perror("minishell: ");
	return (0);
}
