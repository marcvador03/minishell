/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_creation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:32:18 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/19 14:40:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**identify_pipes(char *s_line, t_pipe **p)
{
	char	**pipes;
	int		i;

	pipes = ft_split(s_line, '|');
	if (pipes == NULL)
		return (set_errno(ENOMEM), NULL);
	(*p)->count = 0;
	i = 0;
	while (pipes[i++] != NULL)
		(*p)->count++;
	return (set_flag(*p, 0), pipes);
}

static char	*create_prompt(void)
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

char	*get_input(void)
{
	char	*line;
	char	*line2;
	char	*prompt;

	prompt = create_prompt();
	if (prompt == NULL)
		return (set_errno(ENOMEM), NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), set_errno(ENOMEM), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input());
	add_history(line);
	line2 = ft_strjoin("&&", line);
	return (free_s(prompt), free_s(line), line2);
}
