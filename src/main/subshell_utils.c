/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 19:47:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 23:23:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_pipe(t_pipe *p, char *t_line, int *err);

t_pipe	*p_lstnew(t_shell *sh, char *line, int *err)
{
	t_pipe	*ptr;

	ptr = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr == NULL)
		return (flush_errors("", -1, 0), NULL);
	ptr->r = (t_redirs *)ft_calloc(sizeof (t_redirs), 1);
	if (ptr->r == NULL)
		return (flush_errors("", -1, 0), NULL);
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->head = ptr;
	ptr->sh = sh;
	ptr->r->sh = sh;
	if (get_next_pipe(ptr, line, err) == 2)
		return (free_pipe(ptr), NULL);
	return (ptr);
}

t_pipe	*p_lstlast(t_pipe *pipe)
{
	t_pipe	*tmp;

	if (pipe == NULL)
		return (NULL);
	tmp = pipe;
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

t_pipe	*p_lstadd(t_pipe **pipe, char *line, int *err)
{
	t_pipe	*tmp;
	t_pipe	*new_node;

	new_node = p_lstnew((*pipe)->sh, line, err);
	if (new_node == NULL)
		return (free_pipe(*pipe), NULL);
	else
	{
		tmp = p_lstlast(*pipe);
		tmp->next = new_node;
		*pipe = tmp->next;
		(*pipe)->head = tmp->head;
		(*pipe)->prev = tmp;
	}
	return (tmp->next);
}
