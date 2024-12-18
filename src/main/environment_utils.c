/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:53:05 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/18 10:15:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env(char **env, int offset)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i] != NULL)
		i++;
	new_env = (char **)ft_calloc(sizeof(char *), i + offset + 1);
	if (new_env == NULL)
		return (NULL);
	return (new_env);
}

char	**fill_env(char *envp[])
{
	char	**env;
	int		i;

	if (envp == NULL || envp[0] == NULL)
		return (NULL);
	env = create_env(envp, 0);
	if (env == NULL)
		return (set_gstatus(202), NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		env[i] = ft_strdup(envp[i]);
		if (env[i] == NULL)
			return (set_gstatus(202), NULL);
		i++;
	}
	return (env);
}

char	*create_entry(char *var_name, char *new_value)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(var_name, "=");
	if (tmp == NULL)
		return (NULL);
	res = ft_strjoin(tmp, new_value);
	if (res == NULL)
		return (NULL);
	return (free_s(tmp), res);
}

static char	**search_path(char **env)
{
	int		i;
	char	**res;

	i = 0;
	res = NULL;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0 && ft_strlen(env[i]) > 5)
			res = ft_split(env[i] + 5, ':');
		i++;
	}
	return (res);
}

char	*get_full_path(char *arg0, char **env)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (access(arg0, X_OK) == 0)
		return (ft_strdup(arg0));
	paths = search_path(env);
	if (paths == NULL)
		return (set_gstatus(2), NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(paths), set_gstatus(202), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free_s(cmd_in), set_gstatus(202), NULL);
		if (access(cmd_out, X_OK) == 0)
			return (free_d(paths), free_s(cmd_in), cmd_out);
		i++;
		free_s(cmd_out);
	}
	return (free_d(paths), free_s(cmd_in), set_gstatus(127), NULL);
}
