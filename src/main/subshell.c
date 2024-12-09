/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/11 00:10:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *line);
int	single_cmd(t_pipe *p, char *envp[]);
int	multiple_cmd(t_pipe *p, char *envp[]);

static t_pipe	*fill_pipes(t_pipe *p, char *line, int n)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	while (i < n)
	{
		t_line = ft_strtrim(line, " ");
		if (p == NULL)
			tmp = p_lstnew(t_line);
		else
			tmp = p_lstadd_back(&p, t_line);
		if (tmp == NULL)
			return (free_s(t_line), NULL);
		p = tmp->head;
		free_s(t_line);
		i++;
	}
	return (tmp->head);
}

int	subshell(t_shell *sh, char *envp[])
{
	t_pipe	*p;

	p = NULL;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(p, sh->s_line, sh->p_count);
	if (p == NULL /*|| (p != NULL && p->p_line == NULL)*/)
		return (-1);
	sh->pipes = p->head;
	//free_s(sh->s_line);
	if (sh->p_count == 1)
	{
		if (ft_strncmp(p->args[0], "exit", 4) == 0)
			exit_minishell(sh, EXIT_SUCCESS);
		single_cmd(p, envp);
	}
	else if (multiple_cmd(p, envp) == -1)
		return (free_pipe(sh->pipes), -1);
	return (free_pipe(sh->pipes), 0);
}
