/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/08 16:07:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_shell(t_env *env, t_terms *tcap);
int	close_redir_fd(t_pipe *p);

unsigned int	g_status = 0;

int	main_cmd_return(t_pipe *p, int wstatus)
{
	p = p->head;
	if (WIFEXITED(wstatus))
		g_status = WEXITSTATUS(wstatus);
	return (0);
}

void	exit_minishell_error(t_shell *sh, int status, t_env *env)
{
	if (sh != NULL)
		unset_term_settings(sh->tcap, env);
	if (status != 0)
	{
		ft_putstr_fd("minishell exited with error: ", 2);
		ft_putnbr_fd(status, 2);
		ft_putstr_fd("\n", 2);
		free_sh(sh);
		if (env != NULL)
			free_env(env);
		exit(status);
	}
	else
	{
		printf("exit\n");
		free_sh(sh);
		if (env != NULL)
			free_env(env);
		exit(status);
	}
}

void	exit_minishell(t_pipe *p, t_env *env)
{
	if (p->head == p)
	{
		if (p->sh != NULL && p->head == p)
			unset_term_settings(p->sh->tcap, env);
		printf("exit\n");
		close_redir_fd(p);
		free_sh(p->sh);
		if (env != NULL)
			free_env(env);
		exit(g_status);
	}
	else
		free_sh(p->sh);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_terms	tcap;
//	char	**env;
	t_env	*env;

	if (isatty(STDIN_FILENO) == 0)
		exit_minishell_error(NULL, errno, NULL);
	env = fill_env(envp);
	//if (env == NULL) // to revise default env variables
		env = fill_default_session();
	init_termcaps(&tcap, env);
	set_term_settings(&tcap, env);
	if (argc > 1 || argv == NULL)
		exit_minishell_error(NULL, 1, env);
	while (1)
		if (start_shell(env, &tcap) != 0)
			flush_errors(NULL, g_status);
	return (0);
}
