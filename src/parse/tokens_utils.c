/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:53:29 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 19:32:52 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line)
{
	int		len;

	len = ft_strlen(line);
	if (sh_strpos(line, "&&") == len && sh_strpos(line, "||") == len)
		return (NULL);
	else if (sh_strpos(line, "&&") < sh_strpos(line, "||"))
		return ("&&");
	else if (sh_strpos(line, "||") < sh_strpos(line, "&&"))
		return ("||");
	return (NULL);
}

void	count_brackets(t_shell *sh, char *line)
{
	char	*t_line;

	t_line = line;
	while (*ft_strchr(t_line, '(') != '\0' && *t_line != '\0')
	{
		sh->bracket[0]++;
		t_line = ft_strchr(t_line, '(') + 1;
	}
	sh->bracket[1] = sh->bracket[0];
	t_line = line;
	while (*ft_strchr(t_line, ')') != '\0' && *t_line != '\0')
	{
		sh->bracket[1]--;
		t_line = ft_strchr(t_line, ')') + 1;
	}
}

int	count_tokens(char *line)
{
	int		n;
	int		i;
	int		len;
	char	*t_line;

	if (line == NULL)
		return (0);
	n = 0;
	t_line = line + 2;
	len = ft_strlen(t_line);
	if (sh_strpos(t_line, "&&") == len && sh_strpos(t_line, "||") == len)
		return (1);
	i = 0;
	while (i < len)
	{
		i += sh_strpos(t_line + i, get_tk(t_line + i)) + 2;
		n++;
	}
	return (n);
}

static void	open_quotes_loop(int *cnt, char *str, int i, char c)
{
	if (str[i++] == c)
		*cnt = *cnt + 1;
	return ;
}

int	check_open_q(char *str)
{
	int		i;
	int		cnt[40];
	char	c;

	i = 0;
	cnt[39] = 0;
	cnt[34] = 0;
	while (str[i] != '\0')
	{
		if (str[i] == 39 || str[i] == 34)
		{
			c = str[i];
			cnt[(int)c]++;
			i++;
			while (str[i] != '\0')
				open_quotes_loop(&cnt[(int)c], str, i, c);
			if (str[i] != '\0')
				break ;
		}
		i++;
	}
	if (cnt[39] % 2 != 0 || cnt[34] % 2 != 0)
		return (-1);
	return (0);
}
