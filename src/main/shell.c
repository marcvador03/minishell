/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 18:47:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int status);
char	*create_prompt(t_env *env);
int		subshell(t_shell *sh);
t_shell	*parse_sh(t_shell *sh, char *line, int *pos);
t_shell	*fill_sh(char *line, t_terms *tcap, t_env *env, int *l_status);
int		get_subshell_redirs(t_shell *sh, char **s_line, int *pos);
int		open_redir_fd(t_redirs *r, int *err, char *cmd);
int		close_redir_fd_single(t_redirs *r, int *err, char *cmd);


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
/*static t_shell	*move_sh(t_shell *sh, int *status, int level)
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
}*/

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
		{
			if ((sh->tk == 0 && status == 0) || (sh->tk == 1 && status != 0))
				status = subshell(sh);
			if (sh->up != NULL && sh->next == NULL)
			{
				if (sh->up != NULL && sh->up->r != NULL)
				{
					close_redir_fd_single(sh->up->r, &status, "");
					close_redir_fd_sh(sh->up);
				}
				exit(status);
			}
			sh->pipes = NULL;
		}
		if (sh->exit == 1)
			break;
		sh = sh->next;
		if (sh != NULL)
			sh->l_status = status;
	}
	return (status);
}

/*int	execute_tokens(t_shell *sh, int level, int status)
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
			{
				if (get_fds_redir(sh->r, &sh->l_status) == -1)
					return (close_redir_fd_sh(sh->head), 2);
				status = subshell(sh);
			}
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
}*/

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

static int	get_next_token(t_shell *sh, char *line, t_parse *q)
{
	t_shell	*sub_sh;
	char	c;

	if (sh != sh->head)
	{
		q->i += sh_skip(line + q->i, ' ');
		if (line[q->i] == '|' && line[q->i] == '|')
			sh->tk = 1;
		c = line[q->i];
		while (line[q->i] == c)
		{
			ft_memset(line + q->i, ' ', 1);
			q->i++;
		}
	}
	while (one_of_char(line[q->i], "&,|,(,)") == FALSE && line[q->i] != '\0')
		q->i++;
	if (line[q->i] == '\0')
		return (q->i);
	else if (line[q->i] == '&' || line[q->i] == '|')
	{
		if (line[q->i] == '&' && line[q->i] != line[q->i + 1])
			return (flush_errors("", 210, ""), -1);
		else if (line[q->i + 1] != '\0' && one_of_char(line[q->i + 2], "&,|,)") == TRUE)
				return (flush_errors("", 210, ""), -1);
		else if (line[q->i + 1] == '&' || line[q->i + 1] == '|')
			return (0);
		else if (line[q->i] == '|' && none_of_char(line[q->i + 1], "|,&,(,)") == TRUE)
		{
			q->i++;
			return (get_next_token(sh, line, q));
		}
	}
	else if (line[q->i] == '(')
	{
		q->i++;
		sub_sh = sh_lstadd_down(sh);
		if (sub_sh == NULL)
			return (-1);
 		parse_sh(sub_sh, line, &q->i);
		if (line[q->i] == ')')
		{
			ft_memset(line + q->i, ' ', 1);
			q->prev_pos = q->i;
			while (one_of_char(line[q->i], "<,>") != TRUE && line[q->i] != '\0')
				q->i++;
			if (line[q->i] == '\0')
				q->i = q->prev_pos;
			else
				if (get_subshell_redirs(sh, &line, &q->i) == -1)
					return (-1);
		}
		while (one_of_char(line[q->i], "&,|,(,)") != TRUE && line[q->i] != '\0')
			q->i++;
		return (0);

	}
	return (q->i);
}

t_shell	*parse_sh(t_shell *sh, char *line, int *pos)
{
	t_parse	q;
	
	init_parse(&q);
	q.i = *pos;
	while (line[q.i] != '\0' && line[q.i] != ')')
	{
		q.beg_sep = q.i;
		if (get_next_token(sh, line, &q) == -1)
			return (NULL);
		sh->s_line = ft_substr(line, q.beg_sep, q.i - q.beg_sep);
		sh->s_line = sh_trim_spaces(sh->s_line);
		if (sh->s_line == NULL)
			return (flush_errors("", 202, ""), NULL);
		if (sh->s_line[0] == '\0')
			return (flush_errors("", 210, ""), NULL);
		if (line[q.i] != '\0' && line[q.i] != ')')
			sh = sh_lstadd_back(sh);
	}
	*pos = q.i;
	return (sh);
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
	/*sh = fill_sh(line, tcap, env, l_status);
	if (sh == NULL)
		return (free_s(line), 0);*/
	free_s(line);
	head = sh->head;
	*l_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	*l_status = execute_tokens(head, 0);
	return (free_sh(head), 0);
}
