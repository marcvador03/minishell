/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/26 17:53:35 by mfleury          ###   ########.fr       */
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

static int	exec_process(char *t_cmd, char **args, char **env2)
{
	int	err;

	if (execve(t_cmd, args, env2) == -1)
	{
		err = flush_errors(t_cmd, -1, 0);
		free_s(t_cmd);
		exit(err);
	}
	free_s(t_cmd);
	exit(0);
}

static int	exec_syscmd_multiple(t_pipe *p, t_env *env, char **env2)
{
	char	*t_cmd;
	int		*err;
	int		x;

	err = &p->p_status;
	x = is_absolute_path(p->args[0], err);
	if (x == 0)
		t_cmd = ft_strdup(p->args[0]);
	else if (x == -1)
		t_cmd = get_full_path(p->args[0], env, err);
	else
		return (p->p_status);
	if (t_cmd == NULL)
		return (free_s(t_cmd), p->p_status);
	exec_process(t_cmd, p->args, env2);
	free_s(t_cmd);
	exit(0);
}

static int	exec_syscmd_single(t_pipe *p, t_env *env, char **env2)
{
	char		*t_cmd;
	int			wstatus;
	pid_t		pid;
	int			*err;
	int			x;

	err = &p->p_status;
	init_signal(0, 0);
	x = is_absolute_path(p->args[0], err);
	if (x == 0)
		t_cmd = ft_strdup(p->args[0]);
	else if (x == -1)
		t_cmd = get_full_path(p->args[0], env, err);
	else
		return (p->p_status);
	if (t_cmd == NULL)
		return (free_s(t_cmd), p->p_status);
	p->pid = fork();
	if (p->pid == -1)
		return (free_s(t_cmd), -1);
	if (p->pid == 0)
		exec_process(t_cmd, p->args, env2);
	pid = waitpid(p->pid, &wstatus, 0);
	main_cmd_return(p, wstatus, pid);
	return (free_s(t_cmd), p->p_status);
}

int	exec_cmd(char *cmd, char **args, t_pipe *p, t_env *env)
{
	int			x;
	t_func_arr	call_cmd[6];
	char		**env_arr;
	int			err;

	err = 0;
	if (ft_strncmp(cmd, "exit", max(ft_strlen(cmd), 4)) == 0)
		return (ft_exit(p, args, env));
	x = str_to_enum(cmd, &call_cmd);
	env_arr = get_env_array(env, &err);
	if (env_arr == NULL)
		return (err);
	if (x != -1)
	{
		p->p_status = call_cmd[x](args, env);
		flush_errors(cmd, p->p_status, 0);
	}
	else if (p->sh->p_count == 1)
		exec_syscmd_single(p, env, env_arr);
	else
		exec_syscmd_multiple(p, env, env_arr);
	return (free_d(env_arr), p->p_status);
}
