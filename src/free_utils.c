/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/01 21:15:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	free_sh(t_pipe *p)
{
	int	i;

	if (p->mem_flag & (1 << 5))
	{
		free_s((void *)p->pid);
		unset_flag(p, 5);
	}
	if (p->mem_flag & (1 << 2))
	{
		free_d((void **)p->in_pipes);
		unset_flag(p, 2);
	}
	if (p->mem_flag & (1 << 4))
	{
		free_d((void **)p->fd);
		unset_flag(p, 4);
	}
	if (p->mem_flag & (1 << 3))
	{
		i = 0;
		while (p->args[i] != NULL)
			free_d((void **)p->args[i++]);
		free_s((void **)p->args);
		unset_flag(p, 3);
	}
}
