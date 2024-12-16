/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 00:25:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *line);
int	single_cmd(t_pipe *p, char ***env);
int	multiple_cmd(t_pipe *p, char ***env);

static t_pipe	*fill_pipes(t_pipe *p, char *line, int n)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	while (i < n)
	{
		t_line = line + sh_skip(line, ' ');
		if (p == NULL)
			tmp = p_lstnew(t_line);
		else
			tmp = p_lstadd_back(&p, t_line);
		if (tmp == NULL)
			return (NULL);
		p = tmp->head;
		i++;
	}
	return (tmp->head);
}

int	subshell(t_shell *sh, char ***env)
{
	t_pipe	*p;
	int		len;

	p = NULL;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(p, sh->s_line, sh->p_count);
	if (p == NULL)
		return (-1);
	sh->pipes = p->head;
	if (sh->p_count == 1)
	{
		len = ft_strlen(p->args[0]);
		if (ft_strncmp(p->args[0], "exit", max(len, 4)) == 0)
			ft_exit(sh, p->args, *env);
		single_cmd(p, env);
	}
	else if (multiple_cmd(p, env) == -1)
		return (free_pipe(sh->pipes), -1);
	return (free_pipe(sh->pipes), 0);
}
