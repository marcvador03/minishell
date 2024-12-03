/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 23:55:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
void	count_brackets(t_shell *sh, char *line);
int		execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[]);
int		subshell(t_shell *sh, char *envp[]);

int	get_next_token(t_shell *sh, char *line)
{
	char	*tk1;
	char	*tk2;

	line = sh_strtrim(line, " ", 0);
	tk1 = get_tk(line);
	line = sh_strtrim(line, " ", 2);
	tk2 = get_tk(line);
	sh->s_line = sh_strcut(line, 0, sh_strpos(line, tk2));
	sh->s_line = sh_strtrim(sh->s_line, " ", 0);
	count_brackets(sh, sh->s_line);
	sh->s_line = sh_strtrim(sh->s_line, "(", 0);
	sh->s_line = sh_strtrim(sh->s_line, ")", 0);
	if (sh->s_line == NULL)
		return (free_s(line), ENOMEM);
	if (tk1 != NULL && ft_strncmp(tk1, "||", 2) == 0)
		sh->token = 1;
	return (free_s(line), 0);
}

static void	exec_token_fork(t_shell *sh, t_shell *head, int level, char *envp[])
{
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid == -1)
		perror("minishell: ");
	if (pid == 0)
		exit(execute_tokens(sh, head, ++level, envp));
	waitpid(pid, &wstatus, 0);
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
					perror("minishell: ");
				sh->pipes = NULL;
			}
		}
		else if (sh->bracket[1] < level)
			exit(g_status);
		sh = sh->next;
	}
	return (0);
}
