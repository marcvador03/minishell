/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:39:35 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/02 12:15:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_rd(char *line);
int		skip_quotes(char *str);
int		skip_spaces(char *str);

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

int	exec_syscmd(char *cmd, char **args, char *envp[])
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

int	exec_syscmd_fk(char *cmd, char **args, char *envp[])
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
	int	x;
	int	errnum;
	
	errnum = 0;
	t_func_arr	call_cmd[6];
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

/*char	**create_cmd_names(t_pipe *p)
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
}*/


/*char	*get_args(char **line)
{
	int		i;
	int		pos;
	char	*res;

	if (line == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	res = NULL;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == 34 || (*line)[i] == 39)
			pos = sh_jump_to(*line + i, (*line)[i]);
		else if ((*line)[i] == ' ')
			pos = i + sh_skip(*line + i, ' ');
		if (pos > 0)
		{
			res = sh_strcut2(line, 0, pos);
			return (res);
		}
		i++;
	}
	return (*line);
}

char	**get_cmd_args(char *line)
{
	char	**args;
	int		cnt_args;
	int		i;
	
	i = 0;
	cnt_args = count_args(line);
	args = (char **)ft_calloc(sizeof(char *), cnt_args + 1);
	if (args == NULL)
		return (NULL);
	while (i < cnt_args)
	{
		line = sh_strtrim(&line, " ", 0);
		args[i] = get_args(&line);
		args[i] = sh_strtrim(&args[i], " ", 0);
		i++;
	}
	args[cnt_args] = NULL;
	return (args);
}*/
