/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:53:29 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/07 14:55:35 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

/*void	get_bracket(char *line)
{
	char	*t_line;

	t_line = line;
	
	t_line = ft_strrchr(line, '(');
	t_line = ft_substr(t_line, 1, sh_strpos(t_line, ")"));
	if (t_line == NULL)
		return ;
}*/

/*int	count_p(char *line, int cnt)
{
	char	*str;

	if (ft_strchr(line, '(') == 0 && ft_strchr(line, ')') == 0)
		return (free_s(line), 1);
	else if (ft_strchr(line, '(') > ft_strrchr(line, ')'))
		return (free_s(line), 0);
	while (*line != '\0')
	{
		if (*line == '(')
		{
			str = ft_substr(line + 1, 0, ft_strrchr(line, ')') - line);
			if (count_p(str, cnt) == 0)
				return (free_s(line), 0);
			cnt += count_p(str, cnt);
			break;
		}
		line++;
	}
	return (free_s(line), cnt);
}*/


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

