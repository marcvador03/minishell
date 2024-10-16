/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_prompt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 12:26:04 by mfleury           #+#    #+#             */
/*   Updated: 2024/10/16 23:31:07 by mfleury          ###   ########.fr       */
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

char	**search_path(char *envp[])
{
	int		i;
	char	**res;

	i = 0;
	res = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0 && ft_strlen(envp[i]) > 5)
			res = ft_split(envp[i] + 5, ':');
		i++;
	}
	return (res);
}

char	*get_full_path(char *arg0, char *envp[])
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (access(arg0, F_OK) == 0)
		return (arg0);
	paths = search_path(envp);
	if (paths == NULL)
		return (NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(paths), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free(cmd_in), NULL);
		if (access(cmd_out, F_OK) == 0)
			return (free_d(paths), free(cmd_in), cmd_out);
		i++;
		free(cmd_out);
	}
	return (free_d(paths), free_s(cmd_in), NULL);
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
