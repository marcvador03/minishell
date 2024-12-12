/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 16:25:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_shell(char *envp[], t_terms *tcap);
int	g_status = 0;

int	main_cmd_return(char *cmd, int wstatus)
{
	if (WIFSIGNALED(wstatus) != 0)
		flush_errors(cmd, WTERMSIG(wstatus));
	return (0);
}

void	exit_minishell_error(t_shell *sh, int status)
{
	ft_putstr_fd("minishell exited with error: ", 2);
	ft_putnbr_fd(status, 2);
	ft_putstr_fd("\n", 2);
	unset_term_settings(sh->tcap);
	free_sh(sh);
	exit(status);
}

void	exit_minishell(t_shell *sh, int status)
{
	printf("minishell exited with success\n");
	free_sh(sh);
	exit(g_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_terms	tcap;	

	g_status = 0;
	if (isatty(STDIN_FILENO) == 0)
		exit_minishell_error(NULL, errno);
	init_termcaps(&tcap);
	set_term_settings(&tcap);
	init_signal(1);
	if (argc > 1 || argv == NULL)
		exit_minishell_error(NULL, 1);
	while (1)
		if (start_shell(envp, &tcap) != 0)
			flush_errors(NULL, g_status);
	return (0);
}
