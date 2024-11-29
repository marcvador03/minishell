/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:41:31 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/29 11:20:54 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(char *str);

int	sh_strpos(const char *big, const char *little)
{
	size_t	i;
	int		j;
	char	*str;

	str = (char *)big;
	if (ft_strlen(little) == 0)
		return (ft_strlen(big) + 1);
	i = 0;
	while (*str != '\0')
	{
		if (*str == 34 || *str == 39)
		{
			i += sh_jump_to(str, *str);
			str = str + sh_jump_to(str, *str);
		}
		if (*little == *str)
		{
			j = 0;
			while (little[j] == str[j])
				if (little[++j] == '\0')
					return (i);
		}
		str++;
		i++;
	}
	return (ft_strlen(big) + 1);
}

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

char	*sh_strtrim(char *str[], char *set, char offset)
{
	char *res;
	
	res = ft_strtrim(*str + offset, set);
	free(*str);
	return (res);
}

void	sh_trim_list_strings(char **str, char *set)
{
	int	i;

	if (str == NULL)
		return ;
	i = 0;
	while (str[i] != NULL)
	{
		str[i] = sh_strtrim(&str[i], set, 0);
		i++;
	}
}

char	*sh_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		j;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while (i < len && *big != '\0')
	{
		if (*big == 34 || *big == 39)
			big = big + sh_jump_to((char *)big, *big);	
		if (*little == *big)
		{
			j = 0;
			while (little[j] == big[j] && (i + j++) < len)
				if (little[j] == '\0')
					return ((char *)big);
		}
		big++;
		i++;
	}
	return (0);
}
