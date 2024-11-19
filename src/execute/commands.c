/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/11/19 15:47:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_rd(char *line);

static t_cmd_enum	str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd", "unset", "export", "env", "echo"};
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

int	exec_cmd(char *cmd, char **args, char *envp[])
{
	int			x;
	t_func_arr	call_cmd[2];
	char		*t_cmd;

	/*call_cmd[0] = &ft_cd;
	call_cmd[1] = &ft_pwd;
	call_cmd[2] = &ft_unset;
	call_cmd[3] = &ft_export;
	call_cmd[4] = &ft_env;
	call_cmd[5] = &ft_echo;*/
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (255);
	x = str_to_enum(cmd);
	if (x != -1)
		return (call_cmd[x](args));
	else
	{
		t_cmd = get_full_path(cmd, envp);
		if (t_cmd == NULL)
			return (set_errno(ENOENT), ENOENT);
		else if (execve(t_cmd, args, envp) == -1)
			return (-1);
	}
	return (set_errno(ENOENT), ENOENT);
}

char	**create_cmd_names(t_pipe *p)
{
	char	**res;
	char	*rd;
	int		i;

	res = (char **)ft_calloc(sizeof(char *), p->count + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < p->count)
	{
		rd = get_rd(p->in_pipes[i]);
		res[i] = sh_strcut2(&p->in_pipes[i], 0, sh_strpos(p->in_pipes[i], rd));
		i++;
	}
	res[p->count] = NULL;
	return (set_flag(p, 5), res);
}

char	**get_cmd_args(char *line)
{
	char	**args;

	args = ft_split(line, ' ');
	if (args == NULL)
		return (NULL);
	return (args);
}
