/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/13 09:48:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd_enum	str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd", "unset", "export", "env", "echo"};
	int			i;

	i = 0;
	if (ft_strlen(str) == 0)
		return (-1);
	while (i < END)
	{
		if (ft_strncmp(enum_str[i], str, ft_strlen(str)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	exec_syscmd_multiple(char *cmd, char **args, char *envp[])
{
	char	*t_cmd;
	int		errnum;

	t_cmd = get_full_path(cmd, envp);
	if (t_cmd == NULL)
		return (-1);
	errnum = execve(t_cmd, args, envp);
	if (errnum != 0)
		g_status = errnum;
	free_s(t_cmd);
	return (errnum);
}

static int	exec_syscmd_single(char *cmd, char **args, char *envp[])
{
	char	*t_cmd;
	int		wstatus;
	pid_t	pid;

	init_signal(0, 0);
	wstatus = 0;
	t_cmd = get_full_path(cmd, envp);
	if (t_cmd == NULL)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (free_s(t_cmd), -1);
	if (pid == 0)
		return (execve(t_cmd, args, envp));
	waitpid(pid, &wstatus, 0);
	main_cmd_return(wstatus);
	free_s(t_cmd);
	return (0);
}

int	exec_cmd(char *cmd, char **args, int pcount, char *envp[])
{
	int			x;
	t_func_arr	call_cmd[6];
	int			wstatus;

	call_cmd[0] = &ft_cd;
	call_cmd[1] = &ft_pwd;
	//call_cmd[2] = &ft_unset;
	call_cmd[3] = &ft_export;
	call_cmd[4] = &ft_env;
	call_cmd[5] = &ft_echo;
	x = str_to_enum(cmd);
	if (x != -1)
		wstatus = call_cmd[x](args);
	else if (pcount == 1)
		wstatus = exec_syscmd_single(cmd, args, envp);
	else
		wstatus = exec_syscmd_multiple(cmd, args, envp);
	if (wstatus != 0)
		flush_errors(cmd, wstatus);
	return (g_status);
}
