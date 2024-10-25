/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/25 14:08:44 by mfleury          ###   ########.fr       */
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

char	**get_input(t_shell *sh)
{
	char	*line;
	char	*prompt;
	char	**pipes;

	prompt = create_prompt();
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), set_errno(ENOMEM), NULL);
	if (ft_strncmp(line, "", ft_strlen(line) + 1) == 0)
	{
		pipes = (char **)ft_calloc(sizeof(char *), 2);
		if (pipes == NULL)
			return (free_s(prompt), free_s(line), set_errno(ENOMEM), NULL);
		pipes[0] = ft_strdup("");
		pipes[1] = NULL;
		return (free_s(prompt), free_s(line), set_flag(sh, 2), pipes);
	}
	if (line != NULL && *line != '\0')
		add_history(line);
	pipes = identify_pipes(line);
	if (pipes == NULL)
		return (free(prompt), free(line), set_errno(ENOMEM), NULL);
	return (free_s(prompt), free_s(line), set_flag(sh, 2), pipes);
}
