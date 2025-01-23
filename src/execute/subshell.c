/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/23 19:48:27 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *line);
int	single_cmd(t_pipe *p, t_env *env);
int	multiple_cmd(t_pipe *p, t_env *env);
int	get_next_pipe(t_pipe *p, char *line, t_env *env, int *err);

static int	get_next_pipe_loop(t_pipe *p, char *t_line, int *err, int i)
{
	if (t_line[i] == '|' || t_line[i] == '\0')
	{
		p->p_line = ft_substr(t_line, 0, i);
		p->p_line = sh_trim_spaces(p->p_line);
		if (p->p_line == NULL)
			return (set_status(flush_errors("", 202, ""), err), 2);
		else if (p->p_line[0] == '\0')
			return (set_status(flush_errors("", 205, ""), err), 2);
		if (t_line[i] == '|')
			ft_memset(t_line, ' ', i + 1);
		else
			ft_memset(t_line, ' ', i);
		return (1);
	}
	return (0);
}

static int	get_next_pipe(t_pipe *p, char *t_line, int *err)
{
	int		i;

	i = 0;
	while (t_line[i] != '\0')
	{
		while (t_line[i] == 34 || t_line[i] == 39)
			i += sh_jump_to(t_line + i, t_line[i]);
		if (get_next_pipe_loop(p, t_line, err, i) == 1)
			return ((create_parsing(p)));
		i++;
	}
	p->p_line = ft_strdup(t_line);
	p->p_line = sh_trim_spaces(p->p_line);
	if (p->p_line == NULL)
		return (set_status(flush_errors("", 202, ""), err), 2);
	else if (p->p_line[0] == '\0')
		return (set_status(flush_errors("", 205, ""), err), 2);
	return (create_parsing(p));
}

static t_pipe	*fill_pipes(t_shell *sh, t_pipe *p, int n, int *err)
{
	t_pipe	*tmp;
	int		i;
	char	*t_line;

	i = 0;
	if (sh->s_line == NULL)
		return (NULL);
	while (i < n)
	{
		t_line = sh->s_line + sh_skip(sh->s_line, ' ');
		if (p == NULL)
			tmp = p_lstnew(sh, t_line, sh->env, err);
		else
			tmp = p_lstadd(&p, t_line, sh->env, err);
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
	p = fill_pipes(sh, p, sh->p_count, &status);
	if (p == NULL)
		return (status);
	sh->pipes = p->head;
	if (sh->p_count == 1)
		status = single_cmd(p, sh->env);
	else
		status = multiple_cmd(p, sh->env);
	return (free_pipe(sh->pipes), status);
}
