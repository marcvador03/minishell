/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:53:29 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/13 16:16:20 by mfleury          ###   ########.fr       */
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

