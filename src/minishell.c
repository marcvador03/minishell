/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/02 00:04:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	{
		start_shell(envp);
	}
	return (0);
}
