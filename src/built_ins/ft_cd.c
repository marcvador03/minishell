/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:20:49 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/14 10:12:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*h_abs_path(const char *path)
{
	char	*abs_path;

	abs_path = ft_strdup(path);
	if (!abs_path)
		return (NULL);
	return (abs_path);
}

char	*h_rel_path(const char *path)
{
	char	cur_path[1024];
	char	*rel_path;
	char	*tmp;

	if (getcwd(cur_path, sizeof(cur_path)) == NULL)
		return (NULL);
	rel_path = ft_strjoin(cur_path, "/");
	if (rel_path)
	{
		tmp = ft_strjoin(rel_path, path);
		free(rel_path);
		rel_path = tmp;
	}
	if (!rel_path)
		return (NULL);
	return (rel_path);
}

char	*get_target_path(char **args, char **env)
{
	char	*path;
	char	*home;

	path = args[1];
	if (args[1] != NULL && args[2] != NULL)
		return (NULL);
	if (args[1] == NULL)
	{
		home = sh_getenv(env, "HOME");
		if (!home)
			return (NULL);
		return (h_abs_path(home));
	}
	if (path[0] == '/')
		return (h_abs_path(path));
	else
		return (h_rel_path(path));
}

int	ft_cd(char **args, char **env)
{
	char	*new_path;

	new_path = get_target_path(args, env);
	if (!new_path)
		return (-1);
	if (chdir(new_path) != 0)
	{
		free (new_path);
		return (-1);
	}
	free (new_path);
	return (0);
}
