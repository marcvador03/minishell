/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:31:45 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/28 15:24:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
void	count_brackets(t_shell *sh, char *line);

int	get_next_token(t_shell *sh, char *line)
{
	char	*tk1;
	char	*tk2;

	line = sh_strtrim(&line, " ", 0);
	tk1 = get_tk(line);
	line = sh_strtrim(&line, " ", 2);
	tk2 = get_tk(line);
	sh->s_line = sh_strcut(line, 0, sh_strpos(line, tk2));
	sh->s_line = sh_strtrim(&sh->s_line, " ", 0);
	count_brackets(sh, sh->s_line);
	sh->s_line = sh_strtrim(&sh->s_line, "(", 0);
	sh->s_line = sh_strtrim(&sh->s_line, ")", 0);
	if (sh->s_line == NULL)
		return (free_s(line), ENOMEM);
	if (tk1 != NULL && ft_strncmp(tk1, "||", 2) == 0)
		sh->token = 1;
	return (free_s(line), 0);
}

int	execute_tokens(t_shell *sh, t_shell *head, int level, char *envp[])
{
	pid_t	pid;
	int		wstatus;
	int		errnum;

	errnum = 0;
	while (sh != NULL)
	{
		pid = 0;
		if (sh->bracket[0] > level)
		{
			pid = fork();
			if (pid == -1)
				perror("minishell: ");
			if (pid == 0)
				exit(execute_tokens(sh, head, ++level, envp));
			waitpid(pid, &wstatus, 0);
			return (main_cmd_return(sh, wstatus));
		}
		else if (sh->bracket[0] == level)
		{
			if (sh->token == 0 || (sh->token == 1 && errnum != 0))
			{
				errnum = subshell(sh, sh->pipes, envp);
				if (errnum != 0)
					perror("minishell: ");
			}
		}
		else if (sh->bracket[1] < level)
			exit(0);
		sh = sh->next;
	}
	return (0);
}
