/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 16:34:27 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/11 18:38:06 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_env_loop(char *s)
{
	char	*value;
	char	*res;

	value = getenv(s);
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
	int		len[3];
	
	len[0] = ft_strlen(in);
	len[1] = ft_strlen(line);
	len[2] = ft_strlen(out);
	tmp[0] = ft_substr(line, 0, *i);
	tmp[1] = ft_substr(line, *i + len[0] + 1, len[1]);
	*i = *i + len[0];
	tmp[2] = ft_strjoin(tmp[0], out);
	if (tmp[2] == NULL)
		return (NULL);
	res = ft_strjoin(tmp[2], tmp[1]);
	if (res == NULL)
		return (NULL);
	free_s(out);
	free_s(in);
	free_s(line);
	return (free_s(tmp[0]), free_s(tmp[1]), free_s(tmp[2]), res);
}

char	*get_dollar_in(char *line)
{
	char	*res;
	int		i;

	i = 1;
	while (ft_isalnum(line[i]) == 1 && line[i] != '\0')
		i++;
	res = ft_substr(line + 1, 0, i - 1);
	if (res == NULL)
		return (NULL);
	return (res);
}

char	*expand_env(char *line)
{
	int		i;
	char	*dollar_out;
	char	*dollar_in;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			return (line);
		if (line[i] == '$')
		{
			dollar_in = get_dollar_in(line + i);
			if (dollar_in == NULL)
				return (set_gstatus(202), NULL);
			dollar_out = expand_env_loop(dollar_in);
			line = resize_line(line, dollar_out, dollar_in, &i);
			if (line == NULL)
				return (set_gstatus(202), NULL);
			if (line[i] == '\0')
				return (line);
		}
		i++;
	}
	return (line);
}
