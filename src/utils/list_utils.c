/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 17:25:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 15:50:44 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		get_next_token(t_shell *sh, char *line);
char	*get_next_pipe(t_pipe *p, char **line);

t_shell	*sh_lstnew(char *line)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (NULL);
	/*ptr->pipes = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr->pipes == NULL)
		return (NULL);*/
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
