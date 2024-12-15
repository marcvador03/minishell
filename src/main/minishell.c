/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/15 16:52:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_shell(char ***env, t_terms *tcap);
unsigned int	g_status = 0;

int	main_cmd_return(t_pipe *p, int wstatus)
{
	p = p->head;
	if (WIFEXITED(wstatus))
		g_status = WEXITSTATUS(wstatus);
	return (0);
}

void	exit_minishell_error(t_shell *sh, int status, char **env)
{
	ft_putstr_fd("minishell exited with error: ", 2);
	ft_putnbr_fd(status, 2);
	ft_putstr_fd("\n", 2);
	if (sh != NULL)
		unset_term_settings(sh->tcap, env);
	free_sh(sh);
	if (env != NULL)
		free_d(env);
	exit(status);
}

void	exit_minishell(t_shell *sh, char **env)
{
	printf("minishell exited with success\n");
	if (sh != NULL)
		unset_term_settings(sh->tcap, env);
	free_sh(sh);
	if (env != NULL)
		free_d(env);
	exit(g_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_terms	tcap;
	char	**env;

	g_status = 0;
	if (isatty(STDIN_FILENO) == 0)
		exit_minishell_error(NULL, errno, NULL);
	env = fill_env(envp); 
	if (env == NULL)
		exit_minishell_error(NULL, 1, NULL);
	init_termcaps(&tcap, env);
	set_term_settings(&tcap, env);
	init_signal(1, 0);
	if (argc > 1 || argv == NULL)
		exit_minishell_error(NULL, 1, env);
	while (1)
		if (start_shell(&env, &tcap) != 0)
			flush_errors(NULL, g_status);
	return (0);
}
