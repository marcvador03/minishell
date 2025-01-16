/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:48:32 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/16 19:08:39 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static char	*concat(char *line, int len)
{
	char	*res;
	int		i;

	res = (char *)ft_calloc(sizeof(char), len + 2);
	if (res == NULL)
		return (NULL);
	res[0] = 34;
	i = 1;
	ft_strlcat(res + 1, line, len);
	res[len] = 34;
	res[len + 1] = '\0';
	free_s(line);
	return (res);
}*/

char	*resize_line(char *line, char *out, char *in, int *i)
{
	char	*res;
	char	*tmp[3];
	int		len[4];

	//out = concat(out, ft_strlen(out));
	len[0] = ft_strlen(in);
	len[1] = ft_strlen(line);
	len[2] = ft_strlen(out);
	tmp[0] = ft_substr(line, 0, *i);
	tmp[1] = ft_substr(line, *i + len[0] + 1, len[1]);
	tmp[2] = ft_strjoin(tmp[0], out);
	if (tmp[2] == NULL)
		return (NULL);
	*i = max(0, ft_strlen(tmp[2]) - 1);
	res = ft_strjoin(tmp[2], tmp[1]);
	if (res == NULL)
		return (free_s(res), NULL);
	free_s(out);
	free_s(line);
	return (free_s(tmp[0]), free_s(tmp[1]), free_s(tmp[2]), res);
}

int	ft_isalnum_plus(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else if (c >= 48 && c <= 57)
		return (1);
	else if (c == '@' || c == '_')
		return (1);
	else
		return (0);
}

int	expand_env_strings(char *line, int i, int *flag)
{
	if (line[i] == 34)
		*flag = *flag * (-1);
	while (line[i] == 39 && flag[0] >= 0)
	{
		i += sh_jump_to(line + i, line[i]);
		if (line[i] == 34)
			*flag = *flag * (-1);
	}
	if (line[i] == '\0')
		return (-1);
	return (i);
}
