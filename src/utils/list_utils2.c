/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:48:32 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 19:47:09 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_pipe(t_pipe *p, char **line);

t_pipe	*p_lstnew(char **line)
{
	t_pipe	*ptr;

	ptr = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr == NULL)
		return (NULL);
	if (get_next_pipe(ptr, line) != 0)
		return (NULL);
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->head = ptr;
	return (ptr);
}

static t_pipe	*p_lstlast(t_pipe *pipe)
{
	t_pipe	*tmp;

	if (pipe == NULL)
		return (NULL);
	tmp = pipe;
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

t_pipe	*p_lstadd_back(t_pipe **pipe, char **line)
{
	t_pipe	*tmp;
	t_pipe	*new_node;

	new_node = p_lstnew(line);
	if (new_node == NULL)
		return (NULL);
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
