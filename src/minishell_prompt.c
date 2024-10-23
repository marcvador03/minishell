/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/23 22:15:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*create_prompt()
{
	char	*user;
//	char	*home;
	char	*res;

	user = getenv("USER");
	//home = getenv("HOME");
	if (user == NULL/* || home == NULL*/)
		return (NULL); // exit_error
	res = ft_strjoin(user, "$ ");
	if (res == NULL)
		return (NULL); //exit_error	
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
		return(set_errno(ENOMEM), NULL);
	return (pipes);
}

char	**get_input()
{
	char	*line;
	char	*prompt;
	char	**pipes;

	prompt = create_prompt();
	line = readline(prompt);
	rl_on_new_line();
	free(prompt);
	if (line  == NULL)
		return (NULL);	
	if (ft_strncmp(line, "", ft_strlen(line) + 1) == 0)
	{
		pipes = (char **)ft_calloc(sizeof(char *), 2);
		if (pipes == NULL)
			return (free(line), set_errno(ENOMEM), NULL);
		pipes[0] = ft_strdup("");
		pipes[1] = NULL;
		return (free(line), pipes);	
	}
	if (line != NULL && *line != '\0')
		add_history(line);
	pipes = identify_pipes(line);
	if (pipes == NULL)
		return (free(line), set_errno(ENOMEM), NULL);
	return (free(line), pipes);	
}




