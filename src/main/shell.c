/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 22:07:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int level, int status);
char	*create_prompt(t_env *env);
int		subshell(t_shell *sh);

/*static int	exec_token_fork(t_shell *sh, int level, int status)
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		flush_errors("", -1, "");
	if (pid == 0)
		exit(execute_tokens(sh, ++level, status));
	waitpid(pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (-1);
}*/

static t_shell	*move_sh(t_shell *sh, int *status, int level)
{
	pid_t	pid;
	int		wstatus;

	if ((*status != 0 && sh->tk == 1) || (*status == 0 && sh->tk == 0))
	{
		pid = fork();
		if (pid == -1)
			flush_errors("", -1, "");
		if (pid == 0)
			exit(execute_tokens(sh, ++level, *status));
		waitpid(pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
			*status = WEXITSTATUS(wstatus);
	}
	while (sh != NULL && (sh->bracket[0] - sh->bracket[1] > level))
	{
		sh->l_status = *status;
		sh = sh->next;
	}
	if (sh != NULL)
	{
		sh->l_status = *status;
		sh = sh->next;
	}
	return (sh);
}

int	execute_tokens(t_shell *sh, int level, int status)
{
	if (sh != sh->head)
		status = sh->l_status;
	while (sh != NULL)
	{
		while (sh != NULL && sh->bracket[0] > level)
			sh = move_sh(sh, &status, level);
		if (sh == NULL)
			return (status);
		else if (sh->bracket[0] == level)
		{
			if ((sh->tk == 0 && status == 0) || (sh->tk == 1 && status != 0))
				status = subshell(sh);
			else if (level > 0)
				exit (status);
			sh->pipes = NULL;
		}
		if (sh->bracket[1] > 0 && level > 0)
			exit (status);
		if (sh->exit == 1)
			break ;
		sh = sh->next;
		if (sh != NULL)
			sh->l_status = status;
	}
	return (status);
}

static t_shell	*fill_sh(char *line, t_terms *tcap, t_env *env, int *l_status)
{
	t_parse	tmp;
	t_shell	*sh;
	t_shell	*head;

	sh = NULL;
	head = NULL;
	init_parse(&tmp);
	while (line[tmp.i] != '\0')
	{
		if (sh != NULL)
			head = sh->head;
		if (sh == NULL)
			sh = sh_lstnew(line, env, &tmp.i, l_status);
		else
			sh = sh_lstadd_back(&sh, line, &tmp.i, l_status);
		if (sh == NULL)
			return (free_sh(head), NULL);
		sh->tcap = tcap;
		sh->bracket[0] = tmp.j + sh->bracket[0] - tmp.k;
		tmp.j = sh->bracket[0];
		tmp.k = sh->bracket[1];
	}
	if (sh->bracket[1] != sh->bracket[0])
		return (free_sh(sh->head), flush_errors("", 206, ""), NULL);
	return (sh->head);
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

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env, tcap, l_status);
	if (line == NULL)
		exit_minishell_error(sh, 200, env);
	if (check_open_quotes(line) == -1)
		return (free_s(line), flush_errors("", 201, ""), 0);
	sh = fill_sh(line, tcap, env, l_status);
	if (sh == NULL)
		return (free_s(line), 0);
	free_s(line);
	head = sh->head;
	*l_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	*l_status = execute_tokens(sh, 0, 0);
	return (free_sh(head), 0);
}
