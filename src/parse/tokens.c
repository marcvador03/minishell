/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 20:55:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh, int level, int status);
int		subshell(t_shell *sh);
int		get_tk2(char *line);
void	init_parse(t_parse *q);

/*static int	search_next_bracket(t_shell *sh, char *line, int i)
{
	int	flag[2];

	flag[0] = 0;
	flag[1] = 0;
	while (line[i] == '(' || line[i] == ')' || line[i] == ' ')
	{
		if (line[i] == '(')
		{
			if (one_of_char(line[i + 1], "),<,>,&,|") == TRUE)
				return (flush_errors("", 206, ""), -1);
			sh->bracket[0]++;
			ft_memset(line + i, ' ', 1);
			flag[0] = 1;
		}
		else if (line[i] == ')')
		{
			if (one_of_char(line[i + 1], "(,<,>,&,|") == TRUE)
				return (flush_errors("", 206, ""), -1);
			sh->bracket[1]++;
			ft_memset(line + i, ' ', 1);
			flag[1] = 1;
		}
		i++;
	}
	if (flag[0] > 0 && flag[1] > 0)
		return (flush_errors("", 206, ""), -1);
	return (i);
}*/

static int 	search_next_token(t_shell *sh, char *line, int i)
{
	while (one_of_char(line[i], "&,|") != TRUE && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (i);
	if (line[i] == '&' && one_of_char(line[i + 1], "|,(,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if (line[i] == '&' && line[i + 1] == '&') 
		return (i);
	if (line[i] == '|' && one_of_char(line[i + 1], "&,(,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if (line[i] == '|' && line[i + 1] == '|') 
		return (i);
	return (search_next_token(sh, line, ++i));

}

static int	set_priority(t_shell *sh, char *s_line, t_parse *q)
{
	while (s_line[q->i] == ' ' && s_line[q->i] != '\0')
		q->i++;
	while (s_line[q->i] == '(')
	{
		ft_memset(s_line + q->i, ' ', 1);
		q->i++;
		q->flag_sep = 0;
		sh->bracket[0]++;
	}
	q->i += sh_skip(s_line + q->i, ' ');
	if (s_line[q->i] == '\0')
		return (flush_errors("", 210, ""), -1);
	if (q->flag_sep == 0 && one_of_char(s_line[q->i], "&,|,)") == TRUE)
		return (flush_errors("", 210, ""), -1);
	else if(q->flag_sep == 1 && one_of_char(s_line[q->i], "&,|") != TRUE)
		return (flush_errors("", 210, ""), -1);
	if (q->flag_sep == 1)
	{
		if (s_line[q->i] == '|' && s_line[q->i + 1] == '|')
			sh->tk = 1;
		while (one_of_char(s_line[q->i], "&,|") == TRUE)
		{
			if (q->k == 0 && s_line[q->i] != s_line[q->i + 1])
				return (flush_errors("", 210, ""), -1);
			else if (q->k == 2)
				return (flush_errors("", 210, ""), -1);
			ft_memset(s_line + q->i, ' ', 1);
			q->k++;
			q->i++;	
		}
	}
	while (s_line[q->i] != ')' && s_line[q->i] != '\0')
	{
		if (s_line[q->i] == '(')
			return (flush_errors("", 210, ""), -1);
		else if (s_line[q->i] == '(')
		{
			set_priority(sh, s_line, q);
			if (s_line[q->i] == '\0')
				return (0);
		}
		q->i++;
	}
	while (s_line[q->i] == ')')
	{
		if (s_line[q->i] == '(')
			return (flush_errors("", 210, ""), -1);
		ft_memset(s_line + q->i, ' ', 1);
		q->i++;
		q->flag_jump = 1;
		sh->bracket[1]++;
	}
	// function get_shell_redirections
	if (q->flag_jump == 1)
	{
		if (q->j > 19)
			return (flush_errors("", 210, ""), -1);
		q->i += sh_skip(s_line + q->i, ' ');
		if (s_line[q->i] == '\0')
			return (0);
		else
			return (flush_errors("", 210, ""), -1);
	}

		/*
		if (none_of_char(s_line[q->i], "<,>") == TRUE)
			return (flush_errors("", 250, ""), -1);
		q->k = 0;
		q->prev_pos = q->i;
		while (one_of_char(s_line[q->i], "<,>") == TRUE)
		{
			if (q->k >= 2)
				return (flush_errors("", 250, ""), -1);
			if (s_line[q->i] == '>' && s_line[q->i] == '<')
				return (flush_errors("", 250, ""), -1);
			else if (s_line[q->i] == '<' && s_line[q->i] == '>')
				return (flush_errors("", 250, ""), -1);
			q->k++;
		}
		q->i += sh_skip(s_line, ' ');
		if (s_line[q->i] == '\0')
			return (flush_errors("", 250, ""), -1);
		sh->s_rd[q->j] = ft_substr(s_line, q->prev_pos, q->i - q->prev_pos);
		if (sh->s_rd[q->j] == NULL)
			return (flush_errors("", 202, ""), -1);
		q->prev_pos = q->i;
		while (s_line[q->i] != '\0' && one_of_char(s_line[q->j], "<,>") == TRUE)
			q->i++;
		sh->s_redirs[q->j] = ft_substr(s_line, q->prev_pos, q->i - q->prev_pos);
		if (sh->s_rd[q->j++] == NULL)
			return (flush_errors("", 202, ""), -1);
		if (s_line[q->i] == '\0')
			return (0);
		else if (one_of_char(s_line[q->j], "<,>") == TRUE)
			return (call same function, 0);
	}*/

	return (0);
}


char	*get_next_subshell(t_shell *sh, char *line, int *pos)
{
	t_parse	q;
	char	*res;

	
	init_parse(&q);
	q.i = *pos;
	if (*pos != 0)
		q.flag_sep = 1;
	q.beg_sep = *pos;
	q.i = search_next_token(sh, line, q.i + 2);
	if (q.i == -1)
		return (NULL);
	res = ft_substr(line, *pos, q.i - q.beg_sep);
	res = sh_trim_spaces(res);
	if (res == NULL)
		return (flush_errors("", 202, ""), NULL);
	*pos = q.i;
	q.i = 0;
	if (set_priority(sh, res, &q) == -1)
		return (free_s(res), NULL);
	return (res);
}


/*char	*get_next_subshell(t_shell *sh, char *line, int *pos)
{
	int		i;
	char	*res;

	
	i = *pos;
	if (*pos == 0 && one_of_char(line[*pos], "|,&") == TRUE)
		return (flush_errors("", 204, ""), NULL);
	else if (*pos == 0)
	{
		i = search_next_bracket(sh, line, i);
		if (i == -1)
			return (NULL);
	}
	else if (*pos != 0)
	{
		if (line[i] == '|' && line[i] == '|')
			sh->tk = 1;
		while (one_of_char(line[i], "&,|") == TRUE)
			ft_memset(line + i++, ' ', 1);
		if (one_of_char(line[i], "|,&") == TRUE || line[i] == '\0')
			return (flush_errors("", 204, ""), NULL);
	}
	i = search_next_token(sh, line, i);
	if (i == -1)
		return (NULL);
	res = ft_substr(line, *pos, i - *pos);
	res = sh_trim_spaces(res);
	if (res == NULL)
		return (flush_errors("", 202, ""), NULL);
	*pos = i;
	return (res);
}*/



/*int	get_next_token(t_shell *sh, char *line)
{
	char	*t_line;
	int		pos;

	if (ft_strncmp(line, "||", 2) == 0)
		sh->tk = 1;
	t_line = line + 2;
	pos = get_tk2(t_line);
	sh->s_line = ft_substr(t_line, 0, pos);
	if (sh->s_line == NULL)
		return (free_sh(sh), set_gstatus(202), -1);
	if (sh->s_line[0] == '&' || sh->s_line[0] == '|')
		return (free_sh(sh), set_gstatus(204), -1);
	if (sh_check_empty(sh->s_line) != 0)
		return (free_sh(sh), set_gstatus(204), -1);
	ft_memset(line, ' ', pos + 2);
	if (count_brackets(sh, sh->s_line) == -1)
		return (free_sh(sh), set_gstatus(206), -1);
	return (0);
}*/

static int	exec_token_fork(t_shell *sh, int level, int status)
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
}

static t_shell	*move_sh(t_shell *sh, int *status, int level)
{
	if ((*status != 0 && sh->tk == 1) || (*status == 0 && sh->tk == 0))
		*status = exec_token_fork(sh, level, *status);
	while (sh != NULL && (sh->bracket[0] - sh->bracket[1] > level))
	{
		//x = sh->bracket[0];
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
