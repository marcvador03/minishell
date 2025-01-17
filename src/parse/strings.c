/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:57:43 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/17 14:50:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable(t_pipe *p, char *line, int *i); 
int		ft_isalnum_plus(int c);

/*void	quit_spaces(char *line)
{
	t_quotes	q;

	if (line == NULL)
		return ;
	q.len = ft_strlen(line);	
	q.i = 0;
	q.j = 0;
	while (line[q.i] != '\0')
	{
		while (line[q.i] == 34 || line[q.i] == 39)
			q.i += sh_jump_to(line + q.i, line[q.i]);
		q.j = q.i;
		while (line[q.i] == ' ')
			q.i += sh_skip(line + q.i, line[q.i]);
		if (q.j != q.i)
			quit_spaces_loop(line, &q);
		if (line[q.i] == '\0')
			return ;
		q.i++;
	}
}*/


/*void	quit_quotes(char *line)
{
	t_quotes	q;
	
	if (line == NULL)
		return ;
	q.len = ft_strlen(line);	
	q.i = 0;
	q.j = 0;
	while (line[q.i] != '\0')
	{
		q.j = q.i;
		while (line[q.i] == 34 || line[q.i] == 39)
			q.i += sh_jump_to(line + q.i, line[q.i]);
		if (q.j != q.i)
		{
			expand_env(env, line, &q.i);
			ft_memmove(line + q.i - 1, line + q.i, q.len);
			ft_memmove(line + q.j, line + q.j + 1, q.len);
		}
		if (line[q.i] == '\0')
			return ;
		q.i++;
	}
}*/

static void	quit_spaces(t_quotes *q, char *line, char *f_line)
{
	int	i;

	i = 0;
	q->j = q->i;
	i = sh_skip(line, line[0]);
	if (i == 1)
		return ;
	ft_memmove(line + 1, line + i, q->len);
	if (line[i] != '\0')
		f_line[q->len - i + q->j + 1] = '\0';
	else
		f_line[q->len - i - 1] = '\0';
	q->len = ft_strlen(f_line);	
	q->i = q->j - 1;
	return ;
}

static char	*trim_within_quotes(t_pipe *p, char *line, t_quotes *q)
{
	int	x;
	int	y;

	q->j = q->i;
	q->i += sh_jump_to(line + q->i, line[q->i]);
	x = q->j;
	if (line[q->j] == 39)
		return (line);
	while (x != q->i)
	{
		if (line[x] == '$' && x + 1 != q->i)
			if (ft_isalnum_plus(line[x + 1]) == 1) // _ case
			{
				y = x;
				line = expand_variable(p, line, &x);
				q->len = ft_strlen(line);
				q->i = q->j + sh_jump_to(line + q->j, line[q->j]);
				x = max(y, x);
			}
		x++;
	}
	return (line);
}

char	*trim_input_line(t_pipe *p, char *line)
{
	t_quotes	q;

	if (line == NULL)
		return (NULL);
	q.len = ft_strlen(line);	
	q.i = 0;
	q.j = 0;
	while (line[q.i] != '\0')
	{
		if (line[q.i] == ' ')
			quit_spaces(&q, line + q.i, line);
		if (line[q.i] == '$' && line[q.i + 1] != '\0')
			if (ft_isalnum_plus(line[q.i + 1]) == 1) //$_
			{
				line = expand_variable(p, line, &q.i);
				q.len = ft_strlen(line);	
			}
		while (line[q.i] == 34 || line[q.i] == 39)
		{
			line = trim_within_quotes(p, line, &q);
			ft_memmove(line + q.i - 1, line + q.i, q.len);
			ft_memmove(line + q.j, line + q.j + 1, q.len);
		}
		if (line[q.i] == '\0')
			return (line);
		q.i++;
	}
	return (line);
}

