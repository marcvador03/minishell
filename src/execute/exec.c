/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 18:56:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	exec_syscmd(char *cmd, char **args, char *envp[])
{
	char	*t_cmd;
	int		errnum;

	t_cmd = get_full_path(cmd, envp);
	if (t_cmd == NULL)
		return (ENOENT);
	errnum = execve(t_cmd, args, envp);
	free_s(t_cmd);
	return (errnum);
}

static int	exec_syscmd_fk(char *cmd, char **args, char *envp[])
{
	char	*t_cmd;
	pid_t	pid;

	t_cmd = get_full_path(cmd, envp);
	init_signal(0);
	if (t_cmd == NULL)
		return (ENOENT);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		return (execve(t_cmd, args, envp));
	waitpid(pid, NULL, 0);
	free_s(t_cmd);
	return (0);
}

int	exec_cmd(char *cmd, char **args, int pcount, char *envp[])
{
	int			x;
	int			errnum;
	t_func_arr	call_cmd[6];

	errnum = 0;
	call_cmd[0] = &ft_cd;
	call_cmd[1] = &ft_pwd;
	//call_cmd[2] = &ft_unset;
	call_cmd[3] = &ft_export;
	call_cmd[4] = &ft_env;
	call_cmd[5] = &ft_echo;
	x = str_to_enum(cmd);
	if (x != -1)
		return (call_cmd[x](args));
	else if (pcount == 1)
		errnum = exec_syscmd_fk(cmd, args, envp);
	else
		errnum = exec_syscmd(cmd, args, envp);
	return (errnum);
}
