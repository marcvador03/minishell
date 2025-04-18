/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:00 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 16:20:19 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_s(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	free_d(char **ptr)
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

void	free_pipe(t_pipe *p)
{
	t_pipe	*tmp;

	if (p == NULL)
		return ;
	p = p->head;
	tmp = p;
	while (tmp != NULL)
	{
		if (p->p_line != NULL)
			free_s((void *)p->p_line);
		if (p->r->redirs != NULL)
			free_d(p->r->redirs);
		if (p->r->rd != NULL)
			free_d(p->r->rd);
		if (p->args != NULL)
			free_d(p->args);
		tmp = tmp->next;
		free_s(p->r);
		free_s(p);
		p = tmp;
	}
}

static void	free_sh_sides(t_shell *sh)
{
	if (sh->up != NULL)
		sh->up->down = NULL;
	if (sh->down != NULL)
		free_sh(sh->down);
	if (sh->r != NULL)
	{
		if (sh->r->rd != NULL)
			free_d(sh->r->rd);
		if (sh->r->redirs != NULL)
			free_d(sh->r->redirs);
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
		free_sh_sides(sh);
		free_s(sh->s_line);
		tmp = tmp->next;
		free_s(sh->r);
		free_s((void *)sh);
		sh = tmp;
	}
}
