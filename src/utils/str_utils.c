/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:41:31 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/04 16:48:44 by mfleury          ###   ########.fr       */
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

static char	sh_first_quote(char *str)
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
}

char	*sh_trim_strings(char *s)
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
	
}
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
