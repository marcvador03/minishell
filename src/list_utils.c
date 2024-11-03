/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:25:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/01 21:47:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
int	get_next_token(t_shell *sh, char *line);

t_shell	*sh_lstnew(char *line)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (NULL);
	ptr->pipes = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr->pipes == NULL)
		return (NULL);
	if (get_next_token(ptr, line) != 0)
		return (NULL);	
	ptr->next = NULL;
	ptr->head = ptr;
	return (ptr);
}

t_shell	*sh_lstlast(t_shell *sh)
{
	t_shell	*tmp;

	if (sh == NULL)
		return (NULL);
	tmp = sh;
	while (tmp->next != NULL)
		tmp = tmp->next;
	return (tmp);
}

t_shell	*sh_lstadd_back(t_shell **sh, char *line)
{
	t_shell	*tmp;
	t_shell	*new_node;

	new_node = sh_lstnew(line);
	if (new_node == NULL)
		return (NULL);
	else
	{
		tmp = sh_lstlast(*sh);
		tmp->next = new_node;
		*sh = tmp->next;
		(*sh)->head = tmp->head;
	}
	return (tmp->next);
}
