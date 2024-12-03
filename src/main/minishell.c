/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/03 01:29:11 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_shell(char *envp[]);

void	flush_errors(int err_sig)
{
	if (err_sig == -1)
	{
		g_status = errno + 128;
		perror("minishell: ");
	}
	else if (err_sig > 0)
		g_status = errno + 128;
	return ;
}

int	main_cmd_return(int wstatus)
{
	if (WIFEXITED(wstatus) != 0)
		flush_errors(WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) != 0)
		flush_errors(WTERMSIG(wstatus));
	return (0);
}

void	exit_minishell(t_shell *sh, int status)
{
	//if (errno == 0 || errno == 255)
	printf("Minishell exited with success\n");
	/*else
		perror("Minishell exited with error:\n");*/
	free_sh(sh);
	exit(status);
}

int	main(int argc, char *argv[], char *envp[])
{
	/*char	term_buffer[2048];
	char	*term_type;
	int		success;*/
	g_status = 0;
	init_signal(1);
	if (argc > 1 || argv == NULL)
		return (1);
	/*term_type = getenv("TERM");
	if (term_type == 0)
		exit_minishell(NULL, 1);
	success = tgetent(term_buffer, term_type);
	if (success <= 0)
		exit_minishell(NULL, success);*/
	while (1)
		if (start_shell(envp) != 0)
			perror("minishell: ");
	return (0);
}
