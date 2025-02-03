/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 13:00:25 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/03 21:14:42 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int	get_full_path_check(char *arg0, char **cmd_out)
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
}*/

static char	*full_path_init(char *arg0, t_env *env, int *err, char ***paths)
{
	char	*path_env;
	char	*cmd_in;

	path_env = sh_getenv(env, "PATH", 0);
	if (path_env == NULL || arg0[0] == '\0')
		return (set(flush_errors(arg0, 127, '\0'), err), NULL);
	*paths = ft_split(path_env, ':');
	if (*paths == NULL)
		return (free_s(paths), set(202, err), NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(*paths), set(202, err), NULL);
	return (cmd_in);
}

char	*get_full_path(char *arg0, t_env *env, int *err)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	cmd_in = full_path_init(arg0, env, err, &paths);
	if (cmd_in == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i++], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free_s(cmd_in), set(202, err), NULL);
		if (access(cmd_out, F_OK) == 0)
			return (free_d(paths), free_s(cmd_in), cmd_out);
		*err = errno;
		free_s(cmd_out);
	}
	*err = flush_errors(arg0, 127, '\0');
	return (free_d(paths), free_s(cmd_in), NULL);
}

int	is_absolute_path(char *cmd, int *err)
{
	int	i;

	if (cmd == NULL)
		return (0);
	i = 0;
	if (cmd[0] == '.')
	{
		if (cmd[1] == '\0' || (cmd[1] == '.' && cmd[2] == '\0'))
		{
			*err = flush_errors(cmd, 127, '\0');
			return (127);
		}
	}
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '/')
			return (check_file_directory(cmd, err));
		i++;
	}
	return (-1);
}

int	check_file_directory(char *t_cmd, int *err)
{
	struct stat	statbuf;

	if (stat(t_cmd, &statbuf) != -1)
	{
		if (S_ISDIR(statbuf.st_mode) != 0)
		{
			*err = flush_errors(t_cmd, 126, '\0');
			return (126);
		}
		if (S_ISREG(statbuf.st_mode) == 0)
		{
			*err = flush_errors(t_cmd, 127, '\0');
			return (127);
		}
	}
	else
	{
		*err = flush_errors(t_cmd, -1, '\0');
		return (*err);
	}
	return (0);
}
