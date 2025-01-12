/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:02:14 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/12 19:36:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_token(t_shell *sh, char *line);

int	check_input(char *line)
{
	char	*t_line;
	
	t_line = ft_strtrim(line, " ");
	if (t_line == NULL)
		return (flush_errors("", 202), -1);
	return (0);
}

t_shell	*sh_lstnew(char *line, t_env *env)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (set_gstatus(202), NULL);
	ptr->next = NULL;
	ptr->head = ptr;
	ptr->env = env;
	if (get_next_token(ptr, line) != 0)
		return (NULL);
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

t_shell	*sh_lstadd_back(t_shell **sh, char *line, t_env *env)
{
	t_shell	*tmp;
	t_shell	*new_node;

	new_node = sh_lstnew(line, env);
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
