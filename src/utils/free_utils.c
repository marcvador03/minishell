/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 01:30:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_s(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	free_d(void **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		ptr[i++] = NULL;
	}
	free(ptr);
	ptr = NULL;
}

static void	free_pipe_elts(t_pipe *p)
{
	if (p->p_line != NULL)
		free_s((void *)p->p_line);
	if (p->redirs != NULL)
		free_d((void **)p->redirs);
	if (p->rd != NULL)
		free_s((void *)p->rd);
	if (p->args != NULL)
		free_d((void **)p->args);
}

void	free_pipe(t_pipe *p)
{
	t_pipe	*tmp;

	if (p == NULL)
		return ;
	p = p->head;
	tmp = p;
	while (tmp != NULL)
	{
		free_pipe_elts(p);
		tmp = tmp->next;
		free_s(p);
		p = tmp;
	}
}

void	free_sh(t_shell *sh)
{
	t_shell	*tmp;

	if (sh == NULL)
		return ;
	sh = sh->head;
	tmp = sh;
	while (tmp != NULL)
	{
		free_pipe(sh->pipes);
		free_s(sh->s_line);
		tmp = tmp->next;
		free_s((void *)sh);
		sh = tmp;
	}
}
