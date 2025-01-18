/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:19:07 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/18 10:57:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum_plus(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else if (c >= 48 && c <= 57)
		return (1);
	else if (c == '@' || c == '_' || c == '?')
		return (1);
	else
		return (0);
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
	//*i = max(0, ft_strlen(tmp[2]) - 1);
	*i += len[2];
	res = ft_strjoin(tmp[2], tmp[1]);
	if (res == NULL)
		return (free_s(res), NULL);
	free_s(out);
	free_s(line);
	return (free_s(tmp[0]), free_s(tmp[1]), free_s(tmp[2]), res);
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

char	*expand_variable(t_pipe *p, char *line, int *i)
{
	char	*res;
	char	*dollar_out;
	char	*dollar_in;

	dollar_in = get_dollar_in(line + *i);
	dollar_out = NULL;
	if (dollar_in == NULL)
		return (NULL);
	if (dollar_in[0] == '?')
		dollar_out = ft_itoa(p->sh->l_status);
	else if (dollar_in[0] == '_' && dollar_in[1] == '\0')
		dollar_out = ft_strdup(sh_getenv(p->sh->env, "_"));
	else	
		dollar_out = expand_getenv(dollar_in, p->sh->env);
	res = resize_line(line, dollar_out, dollar_in, i);
	if (res == NULL)
		return (free_s(dollar_in), NULL);
	return (free_s(dollar_in), res);
}
