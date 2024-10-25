/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:07:20 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_minishell(t_shell *sh, int status, int err)
{
	if (err != 0)
		errno = err;
	if (errno == 0)
		printf("Minishell exited with success\n");
	else
		perror("Minishell exited with error:\n");
	free_sh(sh);
	exit(status);
}

void	main_cmd_return(t_shell *sh)
{
	if (errno != 0)
		exit_minishell(sh, EXIT_FAILURE, 0);
	if (sh->flag & (1 << 1))
		exit_minishell(sh, EXIT_SUCCESS, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_shell	sh;

	if (argc > 1 || argv == NULL)
		return (1);
	sh.count = 0;
	sh.flag = 0;
	while (1)
	{
		sh.in_pipes = get_input(&sh);
		if (sh.in_pipes == NULL)
			perror("minishell");
		while (sh.in_pipes[sh.count++] != NULL);
		sh.count--;
		if (sh.in_pipes != NULL && sh.in_pipes[1] == NULL)
		{
			if (ft_strncmp(sh.in_pipes[0], "", ft_strlen(sh.in_pipes[0] + 1) + 1) != 0)
				subshell(&sh, envp);
			else
				free_sh(&sh);
		}
		else
			subshell(&sh, envp);
		main_cmd_return(&sh);
		sh.count = 0;
		sh.flag = 0;
	}
	return (0);
}
