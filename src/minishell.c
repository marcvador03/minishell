/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/14 12:15:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_split(char **ptr)
{
	int	i;

	if (ptr == NULL)
		return ;
	i = 0;
	while (ptr[i] != NULL)
		free(ptr[i++]);
	free(ptr);
}

void	exit_clean(char **ptr)
{
	if (ptr != NULL)
		free_split(ptr);
	exit(EXIT_SUCCESS);
}

void	get_cmd(char *input)
{
	char	**args;

	if (input == NULL)
		return ;
	args = ft_split(input, ' ');
	if (args == NULL)
		exit(EXIT_FAILURE);
	if (ft_strncmp(args[0], "exit", 4) == 0)
		exit_clean(args);
	return (free_split(args));
}

char	*get_input()
{
	char	*line;

	line = readline("#");
	if (line != NULL && *line != '\0')
		add_history(line);
	return (line);	
}

int	main(/*int argc, char *argv[], char *envp[]*/)
{
	char	*input;
	pid_t	pid;
	int		wstatus;
	
	input = NULL;
	while (1)
	{
		input = get_input();
		if (input != NULL)
		{
			pid = fork();
			if (pid == -1)
				exit(EXIT_FAILURE);
			if (pid == 0 && input != NULL)
				get_cmd(input);
			if (waitpid(0, &wstatus, 0) == -1)
				exit(EXIT_FAILURE);
			if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 0)
				exit(EXIT_SUCCESS);
		}
		free(input);
	}
}
