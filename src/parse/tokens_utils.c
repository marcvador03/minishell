/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:53:29 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/16 22:44:42 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_tk2(char *line)
{
	int	len;
	int	pos[2];

	len = ft_strlen(line);
	if (ft_strnstr(line, "&&", len) == 0 && ft_strnstr(line, "||", len) == 0)
		return (len);
	pos[0] = sh_strpos(line, "&&");
	pos[1] = sh_strpos(line, "||");
	if (pos[0] < pos[1])
		return (pos[0]);
	else
		return (pos[1]);
}

int	check_brackets(char *line, int i)
{
	while (line[i] == 34 || line[i] == 39)
		i += sh_jump_to(line + i, line[i]);
	if (line[i] == '\0')
		return (-1);
	i += sh_skip(line + i + 1, ' '); 
	if (line[i] == '\0')
		return (-1);
	if (line[i + 1] == '\0' || line[i + 1] == ')')
		return (-1);
	return (0);
}

int	count_brackets(t_shell *sh, char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
			i += sh_jump_to(line + i, line[i]);
		if (line[i] == '\0')
			return (0);
		if (line[i] == '(')
		{
			if (check_brackets(line, i) == -1)
				return (-1);
			sh->bracket[0]++;
			ft_memset(line + i, ' ', 1);
		}
		else if (line[i] == ')')
		{
			sh->bracket[1]++;
			ft_memset(line + i, ' ', 1);
		}
		i++;
	}
	return (0);
}

static char	*get_tk(char *line)
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

int	check_open_quotes(char *str)
{
	int	i;
	int	x;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == 34 || str[i] == 39)
		{
			x = sh_jump_to(str + i, str[i]);
			if (x == 0)
				return (-1);
			else
				i += x;
		}
		else
			i++;
	}
	return (0);
}
