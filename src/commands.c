/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/17 15:41:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	exec_cmd(char **args, char *envp[])
{
	int			x;
	t_func_arr	call_cmd[2] = {&ft_cd, &ft_pwd};
	char		*cmd;

	if (ft_strncmp(args[0], "exit", 4) == 0)
		return (99);
	x = str_to_enum(args[0]);
	if (x != -1)
		return (call_cmd[x](args));
	else
	{
		cmd = get_full_path(args[0], envp);
		if (cmd == NULL)
			return (free_d(args), ENOENT);
		else if (execve(cmd, args, envp) == -1)
			return (free_d(args), errno);
	}
	return (ENOENT);
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
