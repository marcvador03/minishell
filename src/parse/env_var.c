/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:27 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 23:17:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_getenv(char *s, char **env)
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

static char	*resize_line(char *line, char *out, char *in, int *i)
{
	char	*res;
	char	*tmp[3];
	int		len[4];

	len[0] = ft_strlen(in);
	len[1] = ft_strlen(line);
	len[2] = ft_strlen(out);
	tmp[0] = ft_substr(line, 0, *i);
	tmp[1] = ft_substr(line, *i + len[0] + 1, len[1]);
	tmp[2] = ft_strjoin(tmp[0], out);
	if (tmp[2] == NULL)
		return (NULL);
	*i = ft_strlen(tmp[2]) - 1;
	res = ft_strjoin(tmp[2], tmp[1]);
	if (res == NULL)
		return (NULL);
	free_s(out);
	free_s(line);
	return (free_s(tmp[0]), free_s(tmp[1]), free_s(tmp[2]), res);
}

static char	*get_dollar_in(char *line)
{
	char	*res;
	int		i;

	i = 1;
	if (line[i] == '?')
		return (ft_strdup("?"));
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

static char	*expand_env_loop(char **env, char *line, int *i, int flag)
{
	char	*res;
	char	*dollar_out;
	char	*dollar_in;

	dollar_in = get_dollar_in(line + *i);
	if (dollar_in == NULL)
		return (NULL);
	if (line[*i + 1] == '?')
		dollar_out = ft_itoa(g_status);
	else if (ft_isalnum(line[*i + 1]) == 1)
		dollar_out = expand_getenv(dollar_in, env);
	else if (flag == -1 && ft_isalnum(line[*i + 1]) == 0)
		return (free_s(dollar_in), line);
	else
	{
		ft_memset(line + *i, ' ', 1);
		return (free_s(dollar_in), line);
	}
	res = resize_line(line, dollar_out, dollar_in, i);
	if (res == NULL)
		return (free_s(dollar_in), NULL);
	return (free_s(dollar_in), res);
}

char	*expand_env(char *line, char **env, int x)
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
				return (set_gstatus(202), NULL);
			if (line[i] == '\0')
				return (line);
		}
		i++;
	}
	return (line);
}
