/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/14 16:06:14 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh/*, int i*/, int level);
int		subshell(t_shell *sh);
int		get_tk2(char *line);

int	get_next_token(t_shell *sh, char *line)
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
}


/*static t_shell	*move_sh(t_shell *sh, int n, int *i)
{
	int		j;
	t_shell	*tmp;

	j = 0;
	tmp = sh;
	while (j < n)
	{
		tmp = tmp->next;
		j++;
	}
	*i = n;
	return (tmp);
}*/
static int	exec_token_fork(t_shell *sh, int level)
{
	pid_t	pid;
	int		wstatus;
	int		status;

	status = -1;
	pid = fork();
	if (pid == -1)
		flush_errors("", -1);
	if (pid == 0)
		exit(execute_tokens(sh, ++level));
	wait(&wstatus);
	if (WIFEXITED(wstatus))
		status = WEXITSTATUS(wstatus);
	return (status);
}

static t_shell	*move_sh(t_shell *sh, int *status, int level)
{
	*status = exec_token_fork(sh, level);
	while (sh != NULL && sh->bracket[1] > 0)
		sh = sh->next;
	return (sh);
}

int	execute_tokens(t_shell *sh, int level)
{
	int	status;

	status = 0;
	while (sh != NULL)
	{
		if (sh->bracket[0] > level)
			sh = move_sh(sh, &status, level);
		if (sh == NULL)
			return (status);
		else if (sh->bracket[0] == level)
		{
			if ((sh->tk == 0 && status == 0) || (sh->tk == 1 && status != 0))
				status = subshell(sh);
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

/*int	execute_tokens(t_shell *sh, int i, int level)
{
	while (sh != NULL)
	{
		if (sh->bracket[0] > level)
		{
			sh = move_sh(sh, exec_token_fork(sh, i, level), &i);
			if (sh == NULL)
				return (i);
		}
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && g_status != 0))
			{
				g_status = subshell(sh);
				sh->pipes = NULL;
			}
		}
		if (sh->bracket[1] > 0 && level > 0)
			exit (i);
		if (sh->exit == 1)
			break ;
		sh = sh->next;
		i++;
	}
	return (0);
}*/
