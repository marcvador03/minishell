/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/05 16:50:19 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_cmd_enum	str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd"};
	int			i;

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
	t_func_arr	call_cmd[2];
	char		*cmd;

	call_cmd[0] = &ft_cd;
	call_cmd[1] = &ft_pwd;
	if (ft_strncmp(args[0], "exit", 4) == 0)
		return (255);
	x = str_to_enum(args[0]);
	if (x != -1)
		return (call_cmd[x](args));
	else
	{
		cmd = get_full_path(args[0], envp);
		if (cmd == NULL)
			return (free_d((void **)args), set_errno(ENOENT), ENOENT);
		else if (execve(cmd, args, envp) == -1)
			return (free_d((void **)args), -1);
	}
	return (set_errno(ENOENT), ENOENT);
}

char	**get_cmd_args(char *cmd_in)
{
	char	**args;

	args = ft_split(cmd_in, ' ');
	if (args == NULL)
		return (NULL);
	return (args);
}
