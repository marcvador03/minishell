/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/16 16:31:49 by mfleury          ###   ########.fr       */
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

static cmd_enum str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd"}; 
	int	i;
	
	i = 0;
	while (i < END)
	{
		if (ft_strncmp(enum_str[i], str, ft_strlen(str)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	exec_cmd(char **args)
{
	int		x;
	t_func_arr	call_cmd[2] = {&ft_cd, &ft_pwd};
	int	werror;

	if (ft_strncmp(args[0], "exit", 4) == 0)
		return (99);
	x = str_to_enum(args[0]);
	werror = call_cmd[x](args);
	//ft_printf("%s is an unknown command\n", args[0]);
	return (werror);
}

char	**get_cmd_args()
{
	char	*line;
	char	*prompt;
	char	**args;

	prompt = create_prompt();
	line = readline(prompt);
	free(prompt);
	if (ft_strncmp(line, "", ft_strlen(line) + 1) == 0)
		return (free(line), NULL);
	if (line != NULL && *line != '\0')
		add_history(line);
	args = ft_split(line, ' ');
	if (args == NULL)
		return (free(line), NULL); 
	return (free(line), args);	
}
