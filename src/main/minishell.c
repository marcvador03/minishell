/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 16:03:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_shell(t_env *env, t_terms *tcap, int l_status);
int	close_redir_fd(t_pipe *p);

unsigned int	g_status = 0;

int	main_cmd_return(t_pipe *p, int wstatus, pid_t pid)
{
	p = p->head;
	while (p != NULL)
	{
		if (p->pid == pid && WIFEXITED(wstatus) != 0)
		{
			p->p_status = WEXITSTATUS(wstatus);
			if (p->next == NULL && p->exit == 1)
				p->sh->exit = 1;
			break ;
		}
		p = p->next;
	}
	return (0);
}

void	exit_minishell_error(t_shell *sh, int errnum, t_env *env)
{
	int	cpyerr;

	cpyerr = errnum;
	if (sh != NULL)
		unset_term_settings(sh->tcap, env);
	ft_putstr_fd("minishell exited with error: ", 2);
	if (cpyerr < 200)
		strerror(cpyerr);
	else
		flush_errors("", 200);
	free_sh(sh);
	if (env != NULL)
		free_env(env);
	exit(2);
}

void	exit_minishell(t_pipe *p, t_env *env)
{
	int	status;

	if (p == NULL)
	{
		printf("exit\n");
		if (env != NULL)
			free_env(env);
		exit(0);
	}
	else if (p->sh->p_count == 1)
	{
		if (p->sh != NULL && p->head == p)
			unset_term_settings(p->sh->tcap, env);
		printf("exit\n");
		status = p->p_status;
		close_redir_fd(p);
		free_sh(p->sh);
		if (env != NULL)
			free_env(env);
		exit(status);
	}
	exit(p->p_status);
	return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_terms	tcap;
	t_env	*env;
	int		l_status;

	if (isatty(STDIN_FILENO) == 0)
		exit_minishell_error(NULL, errno, NULL);
	env = fill_env(envp);
	init_termcaps(&tcap, env);
	set_term_settings(&tcap, env);
	if (argc > 1 || argv == NULL)
		exit_minishell_error(NULL, 209, env);
	l_status = 0;
	while (1)
	{
		g_status = 0;
		l_status = start_shell(env, &tcap, l_status);
		rl_on_new_line();
	}
	return (0);
}
