/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/28 16:29:12 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*create_prompt(void)
{
	char	*user;
	char	*res;

	user = getenv("USER");
	if (user == NULL)
		return (set_errno(ENODATA), NULL);
	res = ft_strjoin(user, "$ ");
	if (res == NULL)
		return (set_errno(ENOMEM), NULL);
	return (res);
}

char	**identify_pipes(char *input)
{
	int		i;
	char	x;
	char	**pipes;

	i = 0;
	while (input[i] != '\0')
	{
		x = input[i++];
		if (x == '|' && input[i] == '|')
			return (set_errno(EPERM), NULL);
	}
	pipes = ft_split(input, '|');
	if (pipes == NULL)
		return (set_errno(ENOMEM), NULL);
	return (pipes);
}

int	count_p(char *line, int cnt)
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
}

static void	fill_sh(t_shell **sh, char *line, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (ft_strchr(line, '(') == 0 && ft_strchr(line, ')') == 0)
			sh[i]->s_line = line;
		
		i++;
	}
}

t_shell *get_input()
{
	char	*line;
	char	*prompt;
	int		n;
	t_shell	*sh;

	prompt = create_prompt();
	if (prompt == NULL)
		return (set_errno(ENOMEM), NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), set_errno(ENOMEM), NULL);
	else if (ft_strlen(line) == 1 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input());
	add_history(line);
	n = count_p(ft_strdup(line), 0);
	if (n == 0)
		return (free_s(prompt), free_s(line), set_errno(EINVAL), NULL);
	sh = (t_shell *)ft_calloc(sizeof(t_shell), n);	
	if (sh == NULL)
		return (free_s(prompt), free_s(line), set_errno(ENOMEM), NULL);
	fill_sh(&sh, line, n);
	return (free_s(prompt), free_s(line), sh);
	/*pipes = identify_pipes(line);
	if (pipes == NULL)
		return (free(prompt), free(line), set_errno(ENOMEM), NULL);
	return (free_s(prompt), free_s(line), set_flag(sh, 2), pipes);*/
}
