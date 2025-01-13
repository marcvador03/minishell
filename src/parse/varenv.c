/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 14:28:04 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resize_line(char *line, char *out, char *in, int *i);

static char	*get_dollar_in(char *line)
{
	char	*res;
	int		i;

	i = 1;
	if (line[i] == '?' || line[i] == '_')
		return (ft_strdup(&line[i]));
	if (ft_isdigit(line[i]) == 1)
	{
		res = ft_substr(line, 0, 1);
		if (res == NULL)
			return (NULL);
		return (res);
	}
	while (ft_isalnum(line[i]) == 1 && line[i] != '\0')
		i++;
	res = ft_substr(line + 1, 0, i - 1);
	if (res == NULL)
		return (NULL);
	return (res);
}

static char	*expand_getenv(char *s, t_env *env)
{
	char	*value;
	char	*res;

	value = sh_getenv(env, s);
	if (value != NULL)
		res = ft_strdup(value);
	else
		res = ft_strdup("");
	return (res);
}


static int	remove_dollar(char **line, int i, int flag, char *dollar_in)
{
	if (flag == 100 && ft_isalnum((*line)[i + 1]) == 0)
	{
		free_s(dollar_in);
		return (-1);
	}
	if (ft_isalnum((*line)[i + 1]) == 1 || (*line)[i + 1] == '@')
	{
		ft_memset(*line + i, ' ', 1);
		return (0);
	}
	else if (ft_isalnum((*line)[i + 1]) == 0)
	{
		free_s(dollar_in);
		return (-1);
	}
	return (0);
}

static char	*expand_env_loop(t_env *env, char *line, int *i, int flag)
{
	char	*res;
	char	*dollar_out;
	char	*dollar_in;

	dollar_in = get_dollar_in(line + *i);
	dollar_out = NULL;
	if (dollar_in == NULL)
		return (NULL);
	if (line[*i + 1] == '?')
		dollar_out = ft_itoa(g_status);
	else if (line[*i + 1] == '_')
		dollar_out = ft_strdup(sh_getenv(env, "_"));
	else if (remove_dollar(&line, *i, flag, dollar_in) == -1)
		return (line);
	else if (ft_isalnum(line[*i + 1]) == 1)
		dollar_out = expand_getenv(dollar_in, env);
	res = resize_line(line, dollar_out, dollar_in, i);
	if (res == NULL)
		return (free_s(dollar_in), NULL);
	return (free_s(dollar_in), res);
}

char	*expand_env(char *line, t_env *env, int x)
{
	int	i;
	int	flag;

	i = 0;
	flag = x;
	while (line[i] != '\0')
	{
		if (line[i] == 34)
			flag = flag * (-1);
		while (line[i] == 39 && flag >= 0)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			return (line);
		if ((line[i] == '$' && flag != -1) || \
			(line[i] == '$' && i > 0 && line[i - 1] != '\\'))
		{
			line = expand_env_loop(env, line, &i, flag);
			if (line == NULL)
				return (flush_errors("", 202), NULL);
			if (line[i] == '\0')
				return (line);
		}
		i++;
	}
	return (line);
}
