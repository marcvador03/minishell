/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:41:31 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/05 16:42:58 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sh_strcut(char *str, int start, int end)
{
	char	*res;
	int		i;
	int		j;
	int		len;

	if (str == NULL || end <= start)
		return (NULL);
	len = ft_strlen(str);
	if (start > len)
		return (NULL);
	if (end - start > len)
		end = start + len;
	res = (char *)ft_calloc(sizeof(char), end - start + 1);
	if (res == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (i < end && str[i] != '\0')
		res[j++] = str[i++];
	res[j] = '\0';
	return (res);
}

char	*sh_strstrip(char **str, int start, int end)
{
	char	*res1;
	char	*res2;
	char	*res3;
	int		len;

	len = ft_strlen(*str);
	res1 = sh_strcut(*str, 0, start);
	res2 = sh_strcut(*str, end, len);
	if (res1 == NULL && res2 == NULL)
		return (NULL);
	res3 = ft_strjoin(res1, res2);
	if (res3 == NULL)
		return (NULL);
	return (free_s(*str), free_s(res1), free_s(res2), res3);
}

char	*sh_strcut2(char **str, int start, int end)
{
	char	*res;
	int		i;
	int		j;
	int		len;

	if (str == NULL || end <= start)
		return (NULL);
	len = ft_strlen(*str);
	if (start > len)
		return (NULL);
	if (end - start > len)
		end = start + len;
	res = (char *)ft_calloc(sizeof(char), end - start + 1);
	if (res == NULL)
		return (NULL);
	i = start;
	j = 0;
	while (i < end && str[0][i] != '\0')
		res[j++] = str[0][i++];
	res[j] = '\0';
	*str = sh_strstrip(str, start, end);
	return (res);
}

/*static char	sh_first_quote(char *str)
{
	int		i;
	
	i = 0;
	while (str[i] != '\0' || str[i] != ' ')
	{
		if (str[i] == 34 || str[i] == 39)
			return(str[i]);
		i++;
	}
	return ('\0');
}*/

static int	count_quote_sections(char *s)
{
	int	n;
	int	i;
	int	pos;
		
	n = 0;
	i = 0;
	pos = 0;
	while (s[i] != ' ' && s[i] != '\0')
	{
		if (s[i] == 34 || s[i] == 39)
		{
			if (i != pos)
				n++;
			i += sh_jump_to(s, s[i]);
			n++;
			pos = i;
			if (s[i] == ' ' || s[i] == '\0')
				break;
		}
		i++;
	}
	if (i != pos)
		n++;
	return (n);

}

static char	*split_unquotes(char *s, int i, int pos, int *j)
{
	char	*res;

	res = NULL;
	if (i != pos)
	{
		res = ft_substr(s, pos, i - pos);
		if (res == NULL)
			return (NULL);
		*j = *j + 1;
	}
	return (res);
}

static void	split_quotes_loop(char *s, char **res)
{
	int	i;
	int	j;
	int	pos;

	i = 0;
	j = 0;
	pos = 0;
	while (s[i] != ' ' && s[i] != '\0')
	{
		if (s[i] == 34 || s[i] == 39)
		{
			(res[j]) = split_unquotes(s, i, pos, &j);
			pos = sh_jump_to(s + i + 1, s[i]) + 1;
//			pos = sh_skip(s + pos, s[i]);
			(res[j]) = ft_substr(s, i + 1, pos - i - 2); 
			if (res[j++] == NULL)
				return ;
			i = pos - 1;
		}
		i++;
	}
	(res[j]) = split_unquotes(s, i, pos, &j);
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
	res[n] = NULL;
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
		len += ft_strlen(res[i++]);
	cat_str = (char *)ft_calloc(sizeof(char), len + 1);
	if (cat_str == NULL)
		return (free_d((void **)res), NULL);
	i = 0;
	while (res[i] != NULL)
		ft_strlcat(cat_str, res[i++], len + 1);
	return (free_d((void **)res), cat_str);
}

/*char	*sh_trim_strings(char *s)
{
	char	**str;
	char	*res;
	char	q;
	int		i;
	int		len;

	q = sh_first_quote(s);
	len = 0;
	str = ft_split(s, q);
	if (str == NULL)
		return (NULL) ;
	i = 0;
	while (str[i] != NULL)
		len += ft_strlen(str[i++]);
	res = (char *)ft_calloc(sizeof(char), len + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (str[i] != NULL)
		ft_strlcat(res, str[i++], len + 1);
	return (free_d((void **)str), res);
	
}*/
/*void	sh_trim_strings(char **str)
{
	char	set[2];
	int		i;

	ft_bzero(set, 2);
	i = 0;
	while (*str[i] != ' ' || *str[i] != '\0')
	{
		if (*str[0] == 34 || *str[0] == 39)
			set[0] = *str[0]; 
		*str = sh_strtrim(*str, set, 0);
	}
}*/

void	sh_trim_list_strings(char **str)
{
	int		i;

	if (str == NULL)
		return ;
	i = 0;
	while (str[i] != NULL)
	{
		str[i] = sh_trim_strings(str[i]);
	/*	ft_bzero(set, 2);
		if (*str[i] == 34 || *str[i] == 39)
			set[0] = *str[i]; 
		str[i] = sh_strtrim(str[i], set, 0);*/
		i++;
	}
}
