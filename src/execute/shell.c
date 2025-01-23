/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 19:49:57 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int status);
char	*create_prompt(t_env *env);
int		subshell(t_shell *sh);
t_shell	*parse_sh(t_shell *sh, char *line, int *pos);
t_shell	*fill_sh(char *line, t_terms *tcap, t_env *env, int *l_status);
int		get_subshell_redirs(t_shell *sh, char *t_line, int *pos);
int		open_redir_fd(t_redirs *r, int *err, char *cmd);
int		close_redir_fd_single(t_redirs *r, int *err, char *cmd);
static int	get_next_token(t_shell *sh, char *line, t_parse *q);

static int	execute_fork(t_shell *sh, int status)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		flush_errors("", -1, "");
	if (pid == 0)
		exit(execute_tokens(sh, status));
	waitpid(pid, &wstatus, 0);
	if (sh->up != NULL && sh->up->r != NULL)
	{
		close_redir_fd_single(sh->up->r, &status, "");
		close_redir_fd_sh(sh->up);
	}
	if (WIFEXITED(wstatus))
		status = WEXITSTATUS(wstatus);
	return (status);
}

static void	launch_subshell(t_shell *sh, int *status)
{
	if ((sh->tk == 0 && status == 0) || (sh->tk == 1 && status != 0))
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

int	execute_tokens(t_shell *sh, int status)
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
				status = execute_fork(sh->down, status);
		}
		else if (sh->down == NULL)
			launch_subshell(sh, &status);
		if (sh->exit == 1)
			break ;
		sh = sh->next;
		if (sh != NULL)
			sh->l_status = status;
	}
	return (status);
}

static char	*get_input(t_env *env, t_terms *tcap, int *l_status)
{
	char	*line;
	char	*prompt;

	prompt = create_prompt(env);
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
	{
		free_s(prompt);
		unset_term_settings(tcap, env);
		exit_minishell(NULL, env);
	}
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	if (g_status == 130)
		*l_status = 130;
	g_status = 0;
	add_history(line);
	if (check_forbidden_c(line) == -1)
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	return (free_s(prompt), line);
}

int	start_shell(t_env *env, t_terms *tcap, int *l_status)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;
	int		i;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env, tcap, l_status);
	if (line == NULL)
		exit_minishell_error(sh, 200, env);
	if (check_open_quotes(line) == -1)
		return (free_s(line), flush_errors("", 201, ""), 0);
	sh = sh_lstnew(tcap, env, l_status);
	if (sh == NULL)
		return (set_status(flush_errors("", 202, ""), l_status), 0);
	i = 0;
	sh = parse_sh(sh, line, &i);
	free_s(line);
	head = sh->head;
	*l_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	*l_status = execute_tokens(head, 0);
	return (free_sh(head), 0);
}
