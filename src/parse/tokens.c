/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/13 14:06:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh, int i, int level, char *envp[]);
int		subshell(t_shell *sh, char *envp[]);
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

static int	exec_token_fork(t_shell *sh, int i, int level, char *envp[])
{
	pid_t	pid;
	int		cnt;
	
	cnt = 0;
	pid = fork();
	if (pid == -1)
		perror("minishell: ");
	if (pid == 0)
		exit(execute_tokens(sh, i, ++level, envp));
	waitpid(pid, &cnt, 0);
	g_status = 0;
	if (WIFEXITED(cnt))
		cnt = WEXITSTATUS(cnt);
	return (cnt);
}

static void	move_sh(t_shell **sh, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		*sh = (*sh)->next;
		i++;
	}
}

int	execute_tokens(t_shell *sh, int i, int level, char *envp[])
{
	while (sh != NULL)
	{
		if (sh->bracket[0] > level)
			move_sh(&sh, exec_token_fork(sh, i, level, envp));
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && g_status != 0))
			{
				if (subshell(sh, envp) != 0)
					flush_errors(NULL, g_status);
				sh->pipes = NULL;
			}
		}
		if (sh->bracket[1] < level && level > 0)
			exit (i);
		sh = sh->next;
		i++;
	}
	return (0);
}
