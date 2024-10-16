/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:01:23 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/16 11:38:41 by mfleury          ###   ########.fr       */
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

void	exit_minishell(int status, int err)
{
	if (err != 0)
		errno = err;
	if (errno == 0)
		ft_printf("Minishell exited with success\n");
	else
		ft_printf("Minishell exit with error:\n %s", strerror(errno));
	exit(status);
}

cmd_enum str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd"}; 
	int	i;
	
	i = 0;
	while (i < END)
	{
		if (ft_strncmp(enum_str[i++], str, ft_strlen(str)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	get_cmd(char *input)
{
	char	**args;
	int		x;
	t_func_arr	call_cmd[2] = {&ft_cd, &ft_pwd};

	if (input == NULL)
		return (1); ;
	args = ft_split(input, ' ');
	if (args == NULL)
		return (1); 
	else if (ft_strncmp(args[0], "exit", 4) == 0)
		return (free_split(args), 99);
	x = str_to_enum(args[0]);
	call_cmd[x](args);
	//ft_printf("%s is an unknown command\n", args[0]);
	return (free_split(args), 1);
}

char	*get_input()
{
	char	*line;
	char	*prompt;

	prompt = create_prompt();
	line = readline(prompt);
	free(prompt);
	if (line != NULL && *line != '\0')
		add_history(line);
	return (line);	
}
void	handle_cmd_return(int wstatus)
{
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 0)
		exit_minishell(EXIT_SUCCESS, wstatus);
}	

int	main(/*int argc, char *argv[], char *envp[]*/)
{
	char	*input;
	pid_t	pid[2];
	int		wstatus;
	
	input = NULL;
	while (1)
	{
		pid[0] = fork();
		if (pid[0] == -1)
			exit_minishell(EXIT_FAILURE, 1);
		if (pid[0] == 0)
		{
			input = get_input();
			if (input != NULL)
			{
				get_cmd(input);
				free(input);
			}
		}
		else
		{
			wait(&wstatus);
			handle_cmd_return(wstatus);
		}
	}
	return (0);
}
