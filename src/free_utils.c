/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:10:29 by mfleury          ###   ########.fr       */
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

void	free_sh(t_shell *sh)
{
	int	i;

	if (sh->flag & (1 << 5))
	{
		free_s((void *)sh->pid);
		unset_flag(sh, 5);
	}
	if (sh->flag & (1 << 2))
	{
		free_d((void **)sh->in_pipes);
		unset_flag(sh, 2);
	}
	if (sh->flag & (1 << 4))
	{
		free_d((void **)sh->fd);
		unset_flag(sh, 4);
	}
	if (sh->flag & (1 << 3))
	{
		i = 0;
		while (sh->args[i] != NULL)
			free_d((void **)sh->args[i++]);
		free_s((void **)sh->args);
		unset_flag(sh, 3);
	}
}
