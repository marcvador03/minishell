/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 23:25:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[]);
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
	if (sh->s_line == NULL || sh_check_empty(sh->s_line) != 0)
		return (free_sh(sh), set_gstatus(204), -1);
	ft_memset(line, ' ', pos + 2);
	count_brackets(sh, sh->s_line);
	if (sh->s_line == NULL)
		return (-1);
	return (0);
}

static void	exec_token_fork(t_shell *sh, t_shell *head, int level, char *envp[])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("minishell: ");
	if (pid == 0)
		exit(execute_tokens(sh, head, ++level, envp));
	waitpid(pid, 0, 0);
	return ;
}

int	execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[])
{
	while (sh != NULL)
	{
		if (sh->bracket[0] > level)
			exec_token_fork(sh, head, level, envp);
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && g_status != 0))
			{
				if (subshell(sh, envp) != 0)
					flush_errors(NULL, g_status);
				sh->pipes = NULL;
			}
		}
		else if (sh->bracket[1] < level)
			exit(g_status);
		sh = sh->next;
	}
	return (0);
}
