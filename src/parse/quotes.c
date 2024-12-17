/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 00:48:00 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/18 00:29:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_quote_sections(char *s);

static char	*split_unquotes(char *s, int i, int *pos, int *j)
{
	char	*res;

	res = NULL;
	if (i != *pos)
	{
		res = ft_substr(s, *pos, i - *pos);
		if (res == NULL)
			return (NULL);
		*j = *j + 1;
		*pos = i;
	}
	return (res);
}

static char	*split_move(char *s, int cnt[4], int *j)
{
	char	*res;

	res = NULL;
	if (cnt[0] != cnt[2])
		res = ft_substr(s, cnt[0], cnt[2] - cnt[0] - 1);
	if (res != NULL)
		*j = *j + 1;
	return (res);
}

static void	split_quotes_loop(char *s, char **res)
{
	int	cnt[4];

	cnt[0] = 0;
	cnt[1] = 0;
	cnt[2] = 0;
	while (s[cnt[0]] != ' ' && s[cnt[0]] != '\0')
	{
		if (s[cnt[0]] == 34 || s[cnt[0]] == 39)
		{
			cnt[3] = cnt[1];
			res[cnt[3]] = split_unquotes(s, cnt[0], &cnt[2], &cnt[1]);
			cnt[3] = cnt[1];
			cnt[2] += sh_jump_to(s + cnt[0], s[cnt[0]]);
			cnt[0]++;
			res[cnt[1]] = split_move(s, cnt, &cnt[1]);
			cnt[0] = cnt[2] - 1;
		}
		cnt[0]++;
	}
	cnt[3] = cnt[1];
	res[cnt[3]] = split_unquotes(s, cnt[0], &cnt[2], &cnt[1]);
	cnt[3] = cnt[1];
}

char	**split_quotes(char *s)
{
	int		n;
	char	**res;

	n = count_quote_sections(s);
	res = (char **)ft_calloc(sizeof(char *), n + 1);
	if (res == NULL)
		return (NULL);
	split_quotes_loop(s, res);
	return (res);
}

char	*sh_trim_strings(char *s)
{
	char	**res;
	char	*cat_str;
	int		len;
	int		i;

	res = split_quotes(s);
	if (res == NULL)
		return (NULL);
	i = 0;
	len = 0;
	while (res[i] != NULL)
	{
		len += ft_strlen(res[i++]);
	}
	cat_str = (char *)ft_calloc(sizeof(char), len + 1);
	if (cat_str == NULL)
		return (free_d(res), free_s(s), NULL);
	i = 0;
	while (res[i] != NULL)
		ft_strlcat(cat_str, res[i++], len + 1);
	return (free_d(res), free_s(s), cat_str);
}
