/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:57:43 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/17 19:02:48 by mfleury          ###   ########.fr       */
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
	/*if (i == 1)
		return ;*/
	ft_memmove(line + 1, line + i, q->len);
	if (line[i] != '\0')
		f_line[q->len - i + q->j + 1] = '\0';
	else
		f_line[q->len - i] = '\0';
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

static int 	count_quotes(char *line)
{
	int	n;
	int	prev_pos;
	int	i;
	int			flag_jump;

	if (line == NULL || line[0] == '\0')
		return (-1);
	n = 1;
	i = 0;
	while (line[i] != '\0')
	{
		prev_pos = i;
		flag_jump = 0;
		if (line[i] == ' ')
			i += sh_skip(line + i, ' ');
		if (i - prev_pos >= 1)
			n++;
		while (line[i] == 34 || line[i] == 39)
		{
			i += sh_jump_to(line + i, line[i]);
			flag_jump = 1;
		}
		if (line[i] == '\0')
			return (n);
		if (flag_jump == 0)
			i++;
	}
	return (n);
}

static char	**get_sep_quotes(char *line)
{
	int			n;
	int			i;
	int			j;
	int			prev_pos;
	int			beg_sep;
	char		**seps;
	int			flag_jump;

	if (line == NULL)
		return (NULL);
	n = count_quotes(line);
	seps = (char **)ft_calloc(sizeof(char *), n + 1);
	if (seps == NULL)
		return (flush_errors("", 202), NULL);
	i = 0;
	j = 0;
	beg_sep = 0; 
	while (line[i] != '\0')
	{
		flag_jump = 0;
		prev_pos = i;
		if (line[i] == ' ')
		{
			i += sh_skip(line + i, ' ');
			flag_jump = 1;
		}
		if (i - prev_pos >= 1)
		{
			seps[j] = ft_substr(line, beg_sep, i - beg_sep) ;
			if (seps[j++] == NULL)
				return (flush_errors("", 202), free_d(seps), NULL);
			beg_sep = i;
		}
		while (line[i] == 34 || line[i] == 39)
		{
			i += sh_jump_to(line + i, line[i]);
			flag_jump = 1;
		}
		if (line[i] == '\0')
			break;
		if (flag_jump == 0)
			i++;
	}
	if (i - beg_sep >= 1)
	{
		seps[j] = ft_substr(line, beg_sep, i - beg_sep) ;
		if (seps[j] == NULL)
			return (flush_errors("", 202), free_d(seps), NULL);
	}
	return (seps);
}

char	*trim_line_expand(t_pipe *p, char *line)
{
	t_quotes	q;
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
			q.i = max(0, q.i - 3);
			q.len = ft_strlen(line);	
		}
		if (line[q.i] == '\0')
			return (line);
		q.i++;
	}
	return (line);

}

char	**parse_line(t_pipe *p, char *line)
{
	char	**args;
	int		i;

	args = get_sep_quotes(line);
	if (args == NULL)
		return (flush_errors("", 202), NULL);
	i = -1;
	while (args[++i] != NULL)
		args[i] = trim_line_expand(p, args[i]);
	return (args);
}

