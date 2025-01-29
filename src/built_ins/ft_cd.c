/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:20:49 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/29 13:18:33 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cd_option(char *str);

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

char	*old_path(t_env *env)
{
	char	*old_path;

	old_path = sh_getenv(env, "OLDPWD");
	if (old_path == NULL || *old_path == '\0')
		return (flush_errors("cd", 9, 0), NULL);
	return (ft_strdup(old_path));
}

char	*get_target_path(char **args, t_env *env)
{
	char	*path;
	char	*home;

	path = args[1];
	if (args[1] == NULL)
	{
		home = sh_getenv(env, "HOME");
		if (home == NULL || *home == '\0')
			return (NULL);
		return (h_abs_path(home));
	}
	if (path[0] == '-' && path[1] == '\0')
		return (old_path(env));
	if (path[0] == '/')
		return (h_abs_path(path));
	else
		return (h_rel_path(path));
}

int	chdir_error(char *new_path, char *cur_path, char *arg)
{
	int	err;
	//struct stat	statbuf;

	err = errno;
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(err), STDERR_FILENO);
	free_s(new_path);
	free_s(cur_path);
	return (1);
}

int	ft_cd(char **args, t_env *env)
{
	char	*new_path;
	char	*cur_path;

	if (args[1] != NULL && args[2] != NULL)
		return (flush_errors("cd", 7, 0));
	if (check_cd_option(args[1]) == 2)
		return (2);
	cur_path = getcwd(NULL, 0);
	if (cur_path == NULL)
	{
		flush_errors("cd/getcwd", -1, 0);
		cur_path = ft_strdup(sh_getenv(env, "PWD"));
	}
	new_path = get_target_path(args, env);
	if (new_path == NULL)
		return (free_s(cur_path), flush_errors("cd", 8, 0));
	if (chdir(new_path) != 0)
		return (chdir_error(new_path, cur_path, args[1]));
	sh_updateenv(env, ft_strdup("OLDPWD"), cur_path);
	free_s(new_path);
	new_path = getcwd(NULL, 0);
	if (new_path == NULL)
		flush_errors("cd/getcwd", -1, 0);
	sh_updateenv(env, ft_strdup("PWD"), new_path);
	return (0);
}
