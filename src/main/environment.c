/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 09:05:40 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/14 09:52:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**create_env(char **env, int offset)
{
	int		i;
	char	**new_env;
		
	i = 0;
	while (env[i] != NULL)
		i++;
	new_env = (char **)ft_calloc(sizeof(char *), i + offset);
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

char	*sh_getenv(char **env, char *str)
{
	int	i;
	int	len;
	
	if (env == NULL || str == NULL)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, len) == 0)
			return (env[i] + len);
		i++;
	}
	return (NULL);
}

char	**sh_delenv(char **env, char *str)
{
	int		i;
	int		len;
	char	**new_env;

	if (env == NULL || str == NULL)
		return (NULL);
	new_env = create_env(env, -1);
	if (new_env == NULL)
		return (set_gstatus(202), NULL);
	len = ft_strlen(str);
	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, len) != 0)
		{
			new_env[i] = ft_strdup(env[i]);
			if (new_env[i] == NULL)
				return (set_gstatus(202), NULL);
		}
		i++;
	}
	return (free_d((void **)env), new_env);
}

