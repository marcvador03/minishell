/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:02:14 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/28 12:58:44 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*sh_lstnew(t_terms *tcap, t_env *env, int *l_status)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (set(flush_errors("", 202, 0), l_status), NULL);
	ptr->next = NULL;
	ptr->head = ptr;
	ptr->env = env;
	ptr->l_status = *l_status;
	ptr->tcap = tcap;
	return (ptr);
}

static t_shell	*sh_lstlast(t_shell *sh)
{
	t_shell	*tmp;

	if (sh == NULL)
		return (NULL);
	tmp = sh;
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

t_shell	*sh_lstadd_back(t_shell *sh)
{
	t_shell	*ptr;

	ptr = sh_lstnew(sh->tcap, sh->env, &sh->l_status);
	if (ptr == NULL)
		return (NULL);
	sh = sh_lstlast(sh);
	sh->next = ptr;
	ptr->head = sh->head;
	ptr->up = sh->up;
	return (ptr);
}

t_shell	*sh_lstadd_down(t_shell *sh)
{
	t_shell	*ptr;

	ptr = sh_lstnew(sh->tcap, sh->env, &sh->l_status);
	if (ptr == NULL)
		return (NULL);
	ptr->up = sh;
	sh->down = ptr;
	return (ptr);
}
