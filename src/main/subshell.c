/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/11 19:00:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *line);
int	single_cmd(t_pipe *p, t_env *env);
int	multiple_cmd(t_pipe *p, t_env *env);

static t_pipe	*fill_pipes(t_shell *sh, t_pipe *p, int n)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	if (sh->s_line == NULL)
		return (set_gstatus(202), NULL);
	while (i < n)
	{
		t_line = sh->s_line + sh_skip(sh->s_line, ' ');
		if (p == NULL)
			tmp = p_lstnew(t_line, sh->env);
		else
			tmp = p_lstadd_back(&p, t_line, sh->env);
		if (tmp == NULL)
			return (NULL);
		tmp->sh = sh;
		p = tmp->head;
		i++;
	}
	return (tmp->head);
}

int	subshell(t_shell *sh)
{
	t_pipe	*p;
	int		status;

	p = NULL;
	status = 0;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(sh, p, sh->p_count);
	if (p == NULL)
		return (/*flush_errors(NULL, g_status),*/ g_status);
	sh->pipes = p->head;
	if (sh->p_count == 1)
		status = single_cmd(p, sh->env);
	else
		status = multiple_cmd(p, sh->env);
	return (free_pipe(sh->pipes), status);
}
