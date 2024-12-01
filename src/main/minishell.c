/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/01 19:03:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	term_buffer[2048];
	char	*term_type;
	int		success;
	struct sigaction	sig;

	ft_bzero(&sig, sizeof(sig));
	g_status = 0;
	sig.sa_handler = &signal_handler_main;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGTERM, &sig, NULL);
	if (argc > 1 || argv == NULL)
		return (1);
	term_type = getenv("TERM");
	if (term_type == 0)
		exit_minishell(NULL, 1);
	success = tgetent(term_buffer, term_type);
	if (success <= 0)
		exit_minishell(NULL, success);
	while (1)
		if (start_shell(envp) != 0)
			perror("minishell: ");
	return (0);
}
