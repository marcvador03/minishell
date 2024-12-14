/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 09:05:40 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/14 17:53:45 by mfleury          ###   ########.fr       */
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
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	sh_update_env(char **env, char *str, char *new_value)
{
	int		i;
	int		len;
	char	*tmp[2];
	char	*new_value2;
	
	if (env == NULL || str == NULL)
		return ;
	len = ft_strlen(str);
	i = 0;

	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], str, len) == 0)
		{
			tmp[0] = env[i];
			tmp[1] = ft_strjoin(str, "=");
			new_value2 = ft_strjoin(tmp[1], new_value);
			env[i] = new_value2;
			free_s(tmp[0]);
			free_s(tmp[1]);
			free_s(new_value);
			return ;
		}
		i++;
	}
	//insert new value function
	return;
}

char	**sh_del_env(char **env, char *str)
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

