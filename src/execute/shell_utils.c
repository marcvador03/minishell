/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 01:02:14 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 19:19:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_next_subshell(t_shell *sh, char *line, int *pos);
int		get_fds_redir(t_redirs *r, int *err);

t_shell	*sh_lstnew(t_terms *tcap, t_env *env, int *l_status)
{
	t_shell	*ptr;

	ptr = (t_shell *)ft_calloc(sizeof (t_shell), 1);
	if (ptr == NULL)
		return (set_status(flush_errors("", 202, ""), l_status), NULL);
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

int	check_forbidden_c(char *line)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		if (line[i] == ';' || line[i] == 92 || line[i] == 10 || line[i] == 42)
		{
			flush_errors("", 210, "");
			return (-1);
		}
		i++;
	}
	return (0);
}
