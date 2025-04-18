/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 23:04:56 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/05 19:03:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_tokens(t_shell *sh, int status, t_env **env);
int	subshell(t_shell *sh);

static t_shell	*update_sh_env(t_shell *sh, t_env **env)
{
	*env = sh->env;
	return (sh->next);
}

static int	execute_fork(t_shell *sh, int status, t_env **env)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		flush_errors("", -1, 0);
	if (pid == 0)
		exit (execute_tokens(sh, 0, env));
	waitpid(pid, &wstatus, 0);
	if (sh->up != NULL && sh->up->r != NULL)
	{
		close_redir_fd_single(sh->up->r, &status, "");
		close_redir_fd_sh(sh->up);
	}
	if (WIFEXITED(wstatus) != 0)
		status = WEXITSTATUS(wstatus);
	return (status);
}

static void	launch_subshell(t_shell *sh, int *status)
{
	if ((sh->tk == 0 && *status == 0) || (sh->tk == 1 && *status != 0))
		*status = subshell(sh);
	if (sh->up != NULL && sh->next == NULL)
	{
		if (sh->up != NULL && sh->up->r != NULL)
		{
			close_redir_fd_single(sh->up->r, status, "");
			close_redir_fd_sh(sh->up);
		}
		exit(*status);
	}
	sh->pipes = NULL;
	return ;
}

int	execute_tokens(t_shell *sh, int status, t_env **env)
{
	if (sh != sh->head)
		status = sh->l_status;
	while (sh != NULL && sh->exit != 1)
	{
		if (sh->r != NULL)
		{
			if (get_fds_redir(sh->r, &sh->l_status) == -1)
				return (close_redir_fd_sh(sh->head), 2);
			if (open_redir_fd(sh->r, &status, "") == -1)
				return (close_redir_fd_sh(sh->head), 2);
		}
		if (sh->down != NULL)
		{
			if ((sh->tk == 0 && status == 0) || (sh->tk == 1 && status != 0))
				status = execute_fork(sh->down, status, env);
		}
		else if (sh->down == NULL)
			launch_subshell(sh, &status);
		if (sh->exit == 1)
			break ;
		sh = update_sh_env(sh, env);
		if (sh != NULL)
			sh->l_status = status;
	}
	return (status);
}
