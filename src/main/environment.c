/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 09:05:40 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 09:55:07 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env(char **env, int offset);
char	**fill_env(char *envp[]);
char	*create_entry(char *var_name, char *new_value);

char	*sh_getenv(char **env, char *str)
{
	int		i;
	char	*var_name;
	int		len;
	int		len2;
	int		n;

	if (env == NULL || str == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(str);
	while (env[i] != NULL)
	{
		n = sh_strpos(env[i], "=");
		var_name = ft_substr(env[i], 0, n);
		if (var_name == NULL)
			return (set_gstatus(202), NULL);
		len2 = max(len, ft_strlen(var_name));
		if (ft_strncmp(env[i], str, len2) == 0)
			return (free_s(var_name), env[i] + len + 1);
		free_s(var_name);
		i++;
	}
	return (NULL);
}

char	**sh_add_env(char ***env, char *var_name, char *new_value)
{
	int		i;
	char	**new_env;
	char	*entry;

	if (*env == NULL || var_name == NULL)
		return (NULL);
	new_env = create_env(*env, 1);
	if (new_env == NULL)
		return (set_gstatus(202), NULL);
	entry = create_entry(var_name, new_value);
	if (entry == NULL)
		return (free_d(new_env), set_gstatus(202), NULL);
	i = 0;
	while ((*env)[i] != NULL)
	{
		new_env[i] = ft_strdup((*env)[i]);
		if (new_env[i++] == NULL)
			return (free_d(new_env), free_s(entry), set_gstatus(202), NULL);
	}
	new_env[i] = ft_strdup(entry);
	if (new_env[i] == NULL)
		return (free_d(new_env), free_s(entry), set_gstatus(202), NULL);
	return (free_s(entry), free_d((*env)), new_env);
}

char	**sh_update_env(char ***env, char *var_name, char *new_value)
{
	int		i;
	int		len;
	char	*entry;
	char	*tmp;

	if (*env == NULL || var_name == NULL)
		return (NULL);
	len = ft_strlen(var_name);
	entry = NULL;
	i = 0;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], var_name, len) == 0)
		{
			entry = create_entry(var_name, new_value);
			tmp = (*env)[i];
			(*env)[i] = entry;
			return (free_s(tmp), *env);
		}
		i++;
	}
	return (free_s(entry), sh_add_env(env, var_name, new_value));
}

char	**sh_del_env(char ***env, char *str)
{
	int		i;
	int		j;
	int		len;
	char	**new_env;

	if (*env == NULL || str == NULL)
		return (NULL);
	new_env = create_env(*env, -1);
	if (new_env == NULL)
		return (set_gstatus(202), NULL);
	len = ft_strlen(str);
	i = 0;
	j = i;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], str, len) != 0)
		{
			new_env[j] = ft_strdup((*env)[i]);
			if (new_env[j] == NULL)
				return (set_gstatus(202), NULL);
			j++;
		}
		i++;
	}
	return (free_d(*env), new_env);
}
