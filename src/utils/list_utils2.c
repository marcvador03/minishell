/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:48:32 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 15:25:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_pipe(t_pipe *p, char *line, char **env);

t_pipe	*p_lstnew(char *line, char **env)
{
	t_pipe	*ptr;

	ptr = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr == NULL)
		return (NULL);
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->head = ptr;
	if (get_next_pipe(ptr, line, env) != 0)
		return (free_pipe(ptr), NULL);
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

t_pipe	*p_lstadd_back(t_pipe **pipe, char *line, char **env)
{
	t_pipe	*tmp;
	t_pipe	*new_node;

	new_node = p_lstnew(line, env);
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
