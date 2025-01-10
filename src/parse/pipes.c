/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:53:58 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/10 22:31:02 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_redirs(t_pipe *p, t_env *env);
char	**create_args(t_pipe *p, t_env *env);
int		get_fdin_redir(t_pipe *p);
int		get_fdout_redir(t_pipe *p);

int	count_pipes(char *line)
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

static int	fill_in_pipe(t_pipe *p, t_env *env)
{
	p->redirs = create_redirs(p, env);
	if (p->redirs == NULL)
		return (-1);
	p->args = create_args(p, env);
	if (p->args == NULL)
		return (flush_errors(NULL, -1), -1);
	p->r_fd[INPUT] = get_fdin_redir(p);
	if (p->r_fd[INPUT] == -1)
		return (2);
	p->r_fd[OUTPUT] = get_fdout_redir(p);
	return (0);
}

int	get_next_pipe(t_pipe *p, char *t_line, t_env *env)
{
	int		i;

	i = 0;
	while (t_line[i] != '\0')
	{
		while (t_line[i] == 34 || t_line[i] == 39)
			i += sh_jump_to(t_line + i, t_line[i]);
		if (t_line[i] == '|' || t_line[i] == '\0')
		{
			p->p_line = ft_substr(t_line, 0, i);
			if (p->p_line == NULL || sh_check_empty(p->p_line) != 0)
				return (flush_errors(NULL, 205), -1);
			if (t_line[i] == '|')
				ft_memset(t_line, ' ', i + 1);
			else
				ft_memset(t_line, ' ', i);
			return ((fill_in_pipe(p, env)));
			//return (0);
		}
		i++;
	}
	p->p_line = ft_strdup(t_line);
	return(fill_in_pipe(p, env));
	//return (g_status);
}
