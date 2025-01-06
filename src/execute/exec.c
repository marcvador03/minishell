/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/06 16:53:21 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd_enum	str_to_enum(const char *str)
{
	const char	*enum_str[] = {"cd", "pwd", "unset", "export", "env", "echo"};
	int			i;
	int			len;

	i = 0;
	if (ft_strlen(str) == 0)
		return (-1);
	while (i < END)
	{
		len = ft_strlen(enum_str[i]);
		if (ft_strncmp(enum_str[i], str, max(len, ft_strlen(str))) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	check_directory(char *cmd)
{
	struct stat	statbuf;
	int			n;

	if (stat(cmd, &statbuf) != 0)
		return (0);
	n = S_ISDIR(statbuf.st_mode);
	return (n);
}

static int	exec_syscmd_multiple(char *cmd, char **args, char **env)
{
	char	*t_cmd;
	int		errnum;

	t_cmd = get_full_path(cmd, env);
	if (t_cmd == NULL)
		return (g_status);
	if (check_directory(cmd) != 0)
		return (free_s(t_cmd), 126);
	errnum = execve(t_cmd, args, env);
	if (errnum != 0)
		g_status = errnum;
	free_s(t_cmd);
	return (errnum);
}

static int	exec_syscmd_single(char *cmd, char **args, char **env)
{
	char		*t_cmd;
	int			wstatus;
	pid_t		pid;

	init_signal(0, 0);
	wstatus = 0;
	t_cmd = get_full_path(cmd, env);
	if (t_cmd == NULL)
		return (g_status);
	if (check_directory(cmd) != 0)
		return (free_s(t_cmd), 126);
	pid = fork();
	if (pid == -1)
		return (free_s(t_cmd), -1);
	if (pid == 0)
		return (execve(t_cmd, args, env));
	waitpid(pid, &wstatus, 0);
	kill(pid, SIGINT);
	if (WIFEXITED(wstatus))
		return (free_s(t_cmd), WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (free_s(t_cmd), WTERMSIG(wstatus));
	return (free_s(t_cmd), 0);
}

int	exec_cmd(char *cmd, char **args, t_pipe *p, char ***env)
{
	int			x;
	t_func_arr	call_cmd[6];
	int			wstatus;

	if (ft_strncmp(cmd, "exit", max(ft_strlen(cmd), 4)) == 0)
		return (ft_exit(p, args, *env), g_status);
	call_cmd[0] = &ft_cd;
	call_cmd[1] = &ft_pwd;
	call_cmd[2] = &ft_unset;
	call_cmd[3] = &ft_export;
	call_cmd[4] = &ft_env;
	call_cmd[5] = &ft_echo;
	x = str_to_enum(cmd);
	if (x != -1)
		wstatus = call_cmd[x](args, env);
	else if (p->sh->p_count == 1)
		wstatus = exec_syscmd_single(cmd, args, *env);
	else
		wstatus = exec_syscmd_multiple(cmd, args, *env);
	if (wstatus != 0)
		flush_errors(cmd, wstatus);
	return (g_status);
}
