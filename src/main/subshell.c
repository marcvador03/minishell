/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:08:01 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/24 21:42:00 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		create_parsing(t_pipe *p);
t_pipe	*p_lstnew(t_shell *sh, char *line, int *err);
t_pipe	*p_lstadd(t_pipe **pipe, char *line, int *err);
int		single_cmd(t_pipe *p, t_env *env);
int		multiple_cmd(t_pipe *p, t_env *env);

static int	get_next_pipe_loop(t_pipe *p, char *t_line, int *err, int i)
{
	if (t_line[i] == '|' || t_line[i] == '\0')
	{
		p->p_line = ft_substr(t_line, 0, i);
		p->p_line = sh_trim_spaces(p->p_line);
		if (p->p_line == NULL)
			return (set_status(flush_errors("", 202, 0), err), 2);
		else if (p->p_line[0] == '\0')
			return (set_status(flush_errors("", 210, t_line[i]), err), 2);
		if (t_line[i] == '|')
			ft_memset(t_line, ' ', i + 1);
		else
			ft_memset(t_line, ' ', i);
		return (1);
	}
	return (0);
}

int	get_next_pipe(t_pipe *p, char *t_line, int *err)
{
	int		i;
	int		x;

	i = 0;
	while (t_line[i] != '\0')
	{
		while (t_line[i] == 34 || t_line[i] == 39)
			i += sh_jump_to(t_line + i, t_line[i]);
		x = get_next_pipe_loop(p, t_line, err, i);
		if (x == 1)
			return ((create_parsing(p)));
		else if (x == 2)
			return (2);
		i++;
	}
	p->p_line = ft_strdup(t_line);
	p->p_line = sh_trim_spaces(p->p_line);
	if (p->p_line == NULL)
		return (set_status(flush_errors("", 202, 0), err), 2);
	else if (p->p_line[0] == '\0')
		return (set_status(flush_errors("", 210, '|'), err), 2);
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
			tmp = p_lstnew(sh, t_line, err);
		else
			tmp = p_lstadd(&p, t_line, err);
		if (tmp == NULL)
			return (NULL);
		tmp->sh = sh;
		p = tmp->head;
		i++;
	}
	return (tmp->head);
}

static int	count_pipes(char *line)
{
	int	n;
	int	i;

	if (line == NULL)
		return (0);
	n = 1;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			break ;
		if (line[i] == '|')
		{
			i += sh_skip(line, ' ');
			n++;
		}
		i++;
	}
	return (n);
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
