/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:00 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/19 15:45:18 by mfleury          ###   ########.fr       */
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
		if (p->redirs != NULL)
			free_d(p->redirs);
		if (p->rd != NULL)
			free_d(p->rd);
		if (p->args != NULL)
			free_d(p->args);
		tmp = tmp->next;
		free_s(p);
		p = tmp;
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	if (env == NULL)
		return ;
	env = env->head;
	tmp = env;
	while (tmp != NULL)
	{
		free_s(env->varname);
		free_s(env->value);
		tmp = tmp->next;
		free_s((void *)env);
		env = tmp;
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
