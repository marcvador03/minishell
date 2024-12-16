/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/16 12:10:47 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh, int i, int level, char ***env);
int		subshell(t_shell *sh, char ***env);
int		get_tk2(char *line);

int	get_next_token(t_shell *sh, char *line)
{
	char	*t_line;
	int		pos;

	if (ft_strncmp(line, "||", 2) == 0)
		sh->token = 1;	
	t_line = line + 2;
	pos = get_tk2(t_line);
	sh->s_line = ft_substr(t_line, 0, pos);
	if (sh->s_line == NULL)
		return (free_sh(sh), set_gstatus(202), -1);
	if (sh_check_empty(sh->s_line) != 0)
		return (free_sh(sh), set_gstatus(204), -1);
	ft_memset(line, ' ', pos + 2);
	if (count_brackets(sh, sh->s_line) == -1)
		return (set_gstatus(206), -1);
	if (sh->s_line == NULL)
		return (-1);
	return (0);
}

static int	exec_token_fork(t_shell *sh, int i, int level, char ***env)
{
	pid_t	pid;
	int		cnt;
	int		wstatus;
	
	cnt = 0;
	pid = fork();
	if (pid == -1)
		perror("minishell: ");
	if (pid == 0)
		exit(execute_tokens(sh, i, ++level, env));
	wait(&wstatus);
	g_status = 0;
	cnt = WEXITSTATUS(wstatus);
	return (cnt);
}

static t_shell	*move_sh(t_shell *sh, int n, int *i)
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
}

int	execute_tokens(t_shell *sh, int i, int level, char ***env)
{
	while (sh != NULL)
	{
		if (sh->bracket[0] > level)
		{
			sh = move_sh(sh, exec_token_fork(sh, i, level, env), &i);
			if (sh == NULL)
				return (i);
		}
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && g_status != 0))
			{
				if (subshell(sh, env) != 0)
					flush_errors(NULL, g_status);
				sh->pipes = NULL;
			}
		}
		if (sh->bracket[1] > 0 && level > 0)
			exit (i);
		sh = sh->next;
		i++;
	}
	return (0);
}
