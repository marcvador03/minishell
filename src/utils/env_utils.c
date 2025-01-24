/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:00:56 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/24 10:02:33 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_env *lst)
{
	int		cnt;
	t_env	*tmp;

	cnt = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		cnt++;
	}
	return (cnt);
}

char	**get_env_array(t_env *env, int *err)
{
	int		n;
	int		i;
	char	**env_arr;
	char	*tmp;

	if (env == NULL)
		return (NULL);
	env = env->head;
	n = env_size(env);
	env_arr = (char **)ft_calloc(sizeof(char *), n + 1);
	if (env_arr == NULL)
		return (set_status(flush_errors("", -1, 0), err), NULL);
	i = 0;
	while (env != NULL)
	{
		tmp = ft_strjoin(env->varname, "=");
		env_arr[i++] = ft_strjoin(tmp, env->value);
		if (tmp == NULL || env_arr[i - 1] == NULL)
			return (set_status(flush_errors("", 202, 0), err), NULL);
		free(tmp);
		env = env->next;
	}
	env_arr[n] = NULL;
	return (env_arr);
}

static int	get_full_path_check(char *arg0, char **cmd_out)
{
	if (access(arg0, X_OK) == 0)
	{
		*cmd_out = ft_strdup(arg0);
		return (-1);
	}
	else if (ft_strncmp(arg0, "", max(ft_strlen(arg0), 1)) == 0)
	{
		*cmd_out = ft_strdup("");
		return (-1);
	}
	return (0);
}

char	*get_full_path(char *arg0, t_env *env, int *err)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	paths = ft_split(sh_getenv(env, "PATH"), ':');
	if (paths == NULL)
	{
		if (get_full_path_check(arg0, &cmd_out) == -1)
			return (cmd_out);
		else
			return (free_s(paths), set_status(125, err), NULL);
	}
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(paths), set_status(202, err), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i++], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free_s(cmd_in), set_status(202, err), NULL);
		if (access(cmd_out, R_OK) == 0)
			return (free_d(paths), free_s(cmd_in), cmd_out);
		*err = errno;
		free_s(cmd_out);
	}
	if (get_full_path_check(arg0, &cmd_out) == -1)
		return (free_d(paths), free_s(cmd_in), cmd_out);
	return (free_d(paths), free_s(cmd_in), set_status(127, err), NULL);
}

int	check_directory(char *t_cmd)
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
