/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/15 15:53:01 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd_enum	str_to_enum(const char *str, t_func_arr (*call_cmd)[6])
{
	const char	*enum_str[] = {"cd", "pwd", "unset", "export", "env", "echo"};
	int			i;
	int			len;

	i = 0;
	(*call_cmd)[0] = &ft_cd;
	(*call_cmd)[1] = &ft_pwd;
	(*call_cmd)[2] = &ft_unset;
	(*call_cmd)[3] = &ft_export;
	(*call_cmd)[4] = &ft_env;
	(*call_cmd)[5] = &ft_echo;
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

static int	check_directory(char *t_cmd)
{
	struct stat	statbuf;
	int			n;

	if (stat(t_cmd, &statbuf) != 0)
	{
		if (access(t_cmd, X_OK) == -1)
			return (127);
		else
			return (0);
	}
	n = S_ISDIR(statbuf.st_mode);
	if (n != 0)
		return (126);
	return (0);
}

static int	exec_syscmd_multipl(char *cmd, char **args, t_env *env, char **env2)
{
	char	*t_cmd;
	int		errnum;

	t_cmd = get_full_path(cmd, env);
	if (t_cmd == NULL)
		return (free_s(t_cmd), g_status);
	errnum = check_directory(t_cmd);
	if (errnum != 0)
		return (free_s(t_cmd), errnum);
	errnum = execve(t_cmd, args, env2);
	free_s(t_cmd);
	exit (errnum);
}

static int	exec_syscmd_single(t_pipe *p, t_env *env, char **env2)
{
	char		*t_cmd;
	int			wstatus;
	pid_t		pid;

	init_signal(0, 0);
	t_cmd = get_full_path(p->args[0], env);
	if (t_cmd == NULL)
	{
		p->p_status = g_status;
		return (free_s(t_cmd), p->p_status);
	}
	p->p_status = check_directory(t_cmd);
	if (p->p_status != 0)
		return (free_s(t_cmd), p->p_status);
	wstatus = 0;
	p->pid = fork();
	if (p->pid == -1)
		return (free_s(t_cmd), -1);
	if (p->pid == 0)
		exit(execve(t_cmd, p->args, env2));
	pid = waitpid(p->pid, &wstatus, 0);
	main_cmd_return(p, wstatus, pid);
	return (free_s(t_cmd), 0);
}

int	exec_cmd(char *cmd, char **args, t_pipe *p, t_env *env)
{
	int			x;
	t_func_arr	call_cmd[6];
	int			wstatus;
	char		**env_arr;

	if (ft_strncmp(cmd, "exit", max(ft_strlen(cmd), 4)) == 0)
		return (ft_exit(p, args, env));
	x = str_to_enum(cmd, &call_cmd);
	env_arr = get_env_array(env);
	if (env_arr == NULL)
		return (g_status);
	if (x != -1)
	{
		wstatus = call_cmd[x](args, env);
		p->p_status = wstatus;
	}
	else if (p->sh->p_count == 1)
		wstatus = exec_syscmd_single(p, env, env_arr);
	else
		wstatus = exec_syscmd_multipl(cmd, args, env, env_arr);
	p->p_status = flush_errors(cmd, wstatus);
	return (free_d(env_arr), p->p_status);
}
