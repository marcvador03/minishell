/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/01 22:02:45 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_minishell(t_pipe *p, int status, int err)
{
	if (err != 0)
		errno = err;
	if (errno == 0 || errno == 255)
		printf("Minishell exited with success\n");
	else
		perror("Minishell exited with error:\n");
	free_sh(p);
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
