/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:27 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/16 10:29:46 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*resize_line(char *line, char *out, char *in, int *i);
int		ft_isalnum_plus(int c);
int		expand_env_strings(char *line, int i, int *flag);

static char	*get_dollar_in(char *line)
{
	char	*res;
	int		i;

	i = 1;
	if (line[i] == '?')
		return (ft_strdup("?"));
	else if (line[i] == '_' && ft_isalnum_plus(line[i + 1]) == 0)
		return (ft_strdup("_"));
	if (ft_isdigit(line[i]) == 1)
	{
		res = ft_substr(line, 0, 1);
		if (res == NULL)
			return (NULL);
		return (res);
	}
	while ((ft_isalnum(line[i]) == 1 || line[i] == '_') && line[i] != '\0')
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

static int	remove_dollar(char **line, int i, char *dollar_in, int *flag)
{
	if (ft_isalnum_plus((*line)[i + 1]) == 1 && *flag != -1)
	{
		ft_memset(*line + i, ' ', 1);
		return (0);
	}
	else if (((*line)[i + 1] == 34 || (*line)[i + 1] == 39))
	{
		if ((*line)[i + 1] == 39)
			*flag = *flag * (-1);
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

static char	*expand_env_loop(t_env *env, char *line, int *i, int (*flags)[2])
{
	char	*res;
	char	*dollar_out;
	char	*dollar_in;

	dollar_in = get_dollar_in(line + *i);
	dollar_out = NULL;
	if (dollar_in == NULL)
		return (NULL);
	if (dollar_in[0] == '?')
		dollar_out = ft_itoa((*flags)[1]);
	else if (dollar_in[0] == '_' && dollar_in[1] == '\0')
		dollar_out = ft_strdup(sh_getenv(env, "_"));
	else if (remove_dollar(&line, *i, dollar_in, flags[0]) == -1)
		return (line);
	else if (ft_isalnum(line[*i + 1]) == 1)
		dollar_out = expand_getenv(dollar_in, env);
	res = resize_line(line, dollar_out, dollar_in, i);
	if (res == NULL)
		return (free_s(dollar_in), NULL);
	return (free_s(dollar_in), res);
}

char	*expand_env(t_env *env, char *line, int x, int l_status)
{
	int	i;
	int	flag[2];

	i = 0;
	flag[0] = x;
	flag[1] = l_status;
	while (line[i] != '\0')
	{
		i = expand_env_strings(line, i, &flag[0]);
		if (i == -1)
			return (line);
		if (line[i] == '$')
		{
			if ((flag[0] != -1) || (i > 0 && line[i - 1] != '\\'))
			{
				line = expand_env_loop(env, line, &i, &flag);
				if (line == NULL)
					return (flush_errors("", 202), NULL);
				if (line[i] == '\0')
					return (line);
			}
		}
		i++;
	}
	return (line);
}
