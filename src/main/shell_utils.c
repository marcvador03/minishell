/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:02:14 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 21:11:25 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_next_subshell(t_shell *sh, char *line, int *pos);

t_shell	*sh_lstnew(char *line, t_env *env, int *pos, int *l_status)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (set_status(flush_errors("", 202, ""), l_status), NULL);
	ptr->next = NULL;
	ptr->head = ptr;
	ptr->env = env;
	ptr->l_status = *l_status;
	ptr->s_line = get_next_subshell(ptr, line, pos);
	if (ptr->s_line == NULL) // check also line has not only spaces
		return (free_sh(ptr), NULL);
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

t_shell	*sh_lstadd_back(t_shell **sh, char *line, int *pos, int *l_status)
{
	t_shell	*tmp;
	t_shell	*new_node;

	new_node = sh_lstnew(line, (*sh)->env, pos, l_status);
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

int	check_forbidden_c(char *line)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		if (line[i] == ';' || line[i] == 92 || line [i] == 10)
		{
			flush_errors("", 210, "");
			return (-1);
		}
		i++;
	}
	return (0);
}
