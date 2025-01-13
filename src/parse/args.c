/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:52:16 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 16:25:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(char *line)
{
	int	n;
	int	i;

	if (line == NULL)
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
	sh_updateenv(env, ft_strdup("_"), ft_strdup((*args)[n - 1]));
	if (ft_strncmp((*args)[0], "exit", max(ft_strlen((*args)[0]), 4)) == 0)
		p->exit = 1;
	return (0);
}

char	**create_args(t_pipe *p, t_env *env)
{
	char	**args;
	int		i;
	int		n;
	char	*t_line[3];

	t_line[0] = p->p_line + sh_skip(p->p_line, ' ');
	n = count_args(t_line[0]);
	args = (char **)ft_calloc(sizeof(char *), n + 1);
	if (args == NULL)
		return (flush_errors("", -1), NULL);
	i = 0;
	while (i < n)
	{
		args[i] = get_args(t_line[0]);
		if (args[i] == NULL)
			return (free_d(args), flush_errors("", 202), NULL);
		t_line[0] = t_line[0] + ft_strlen(args[i]);
		args[i] = expand_env(env, args[i], 1, p->sh->l_status);
		args[i] = sh_trim_strings(args[i]);
		if (args[i++] == NULL)
			return (free_d(args), NULL);
	}
	finish_args_creation(p, env, &args, n);
	return (args);
}
