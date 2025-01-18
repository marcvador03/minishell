/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:52:16 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/18 00:36:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**parse_line(t_pipe *p, char *line);

/*static int	count_args(char *line)
{
	int	n;
	int	i;

	if (line == NULL || line[0] == '\0')
		return (0);
	n = 1;
	i = 0;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			return (n);
		if (line[i] == ' ')
		{
			i += sh_skip(line + i, ' ') - 1;
			if (line[i + 1] == '\0')
				return (n);
			n++;
		}
		i++;
	}
	return (n);
}

static char	*get_args(char *line)
{
	int		i;
	char	*res;

	if (line == NULL)
		return (NULL);
	i = 0;
	res = NULL;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == ' ')
		{
			i += sh_skip(line + i, ' ');
			res = ft_substr(line, 0, i);
			return (res);
		}
		else if (line[i] == '\0')
			return (ft_strdup(line));
		i++;
	}
	return (ft_strdup(line));
}

static int	finish_args_creation(t_pipe *p, t_env *env, char ***args, int n)
{
	(*args)[n] = NULL;
	if (n > 1)
	{
		sh_updateenv(env, ft_strdup("_"), ft_strdup((*args)[0]));
		if (ft_strncmp((*args)[0], "exit", max(ft_strlen((*args)[0]), 4)) == 0)
			p->exit = 1;
	}
	return (0);
}

char	*create_args_loop(char **t_line)
{
	char	*arg;

	arg = get_args(*t_line);
	if (arg == NULL)
		return (flush_errors("", 202), NULL);
	*t_line = *t_line + ft_strlen(arg);
	arg = sh_trim_strings(arg);
	if (arg == NULL)
		return (NULL);
	return (arg);
}

char	**create_args(t_pipe *p, t_env *env)
{
	char	**args;
	int		i;
	int		n;
	char	*t_line;

	trim_input_line(p, p->p_line);
	p->p_line = expand_env(env, p->p_line, 1, p->sh->l_status);
	t_line = p->p_line + sh_skip(p->p_line, ' ');
	n = count_args(t_line);
	if (n == 0)
		p->empty_arg = 1;
	args = (char **)ft_calloc(sizeof(char *), n + 1);
	if (args == NULL)
		return (flush_errors("", -1), NULL);
	i = 0;
	while (i < n)
	{
		args[i] = create_args_loop(&t_line);
		if (args[i++] == NULL)
			return (free_d(args), NULL);
	}
	finish_args_creation(p, env, &args, n);
	return (args);
}*/
char	**create_args(t_pipe *p)
{
	char	**args;
	char	*tmp;
	
	tmp = p->p_line;
	p->p_line = ft_strtrim(p->p_line, " ");
	free_s(tmp);
	args = parse_line(p, p->p_line);
	if (args == NULL)
		return (NULL);
	/*n = count_args(p->p_line);
	if (n == 0)
		p->empty_arg = 1;
	args = (char **)ft_calloc(sizeof(char *), n + 1);
	if (args == NULL)
		return (flush_errors("", -1), NULL);
	i = 0;
	while (i < n)
	{
		args[i] = create_args_loop(&p->p_line);
		if (args[i++] == NULL)
			return (free_d(args), NULL);
	}
	finish_args_creation(p, env, &args, n);*/
	return (args);
}
