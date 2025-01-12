/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/12 19:27:15 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *line);
int	single_cmd(t_pipe *p, t_env *env);
int	multiple_cmd(t_pipe *p, t_env *env);
int	get_next_pipe(t_pipe *p, char *line, t_env *env);

static t_pipe	*p_lstnew(char *line, t_env *env)
{
	t_pipe	*ptr;

	ptr = (t_pipe *)ft_calloc(sizeof (t_pipe), 1);
	if (ptr == NULL)
		return (NULL);
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->head = ptr;
	if (get_next_pipe(ptr, line, env) == 2)
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

static t_pipe	*p_lstadd_back(t_pipe **pipe, char *line, t_env *env)
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

static t_pipe	*fill_pipes(t_shell *sh, t_pipe *p, int n)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	if (sh->s_line == NULL)
		return (set_gstatus(202), NULL);
	while (i < n)
	{
		t_line = sh->s_line + sh_skip(sh->s_line, ' ');
		if (p == NULL)
			tmp = p_lstnew(t_line, sh->env);
		else
			tmp = p_lstadd_back(&p, t_line, sh->env);
		if (tmp == NULL)
			return (NULL);
		tmp->sh = sh;
		p = tmp->head;
		i++;
	}
	return (tmp->head);
}

int	subshell(t_shell *sh)
{
	t_pipe	*p;
	int		status;

	p = NULL;
	status = 0;
	sh->p_count = count_pipes(sh->s_line);
	p = fill_pipes(sh, p, sh->p_count);
	if (p == NULL)
		return (g_status);
	sh->pipes = p->head;
	if (sh->p_count == 1)
		status = single_cmd(p, sh->env);
	else
		status = multiple_cmd(p, sh->env);
	return (free_pipe(sh->pipes), status);
}
