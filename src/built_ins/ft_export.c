/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:54:49 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/14 10:09:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

int	find_env_var(const char *name)
{
	int	i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, len) == 0 &&
			environ[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	add_env_var(const char *var)
{
	int	index;
	int	len;
	char	*new_env;
	char	**my_env;

	index = find_env_var(var);
	new_env = ft_strdup(var);
	if (!new_env)
	{
		exit (1);
	}
	if (index >= 0)
	{
		free(environ[index]);
		environ[index] = new_env;
	}
	else
	{
		len = 0;
		while (environ[len] != NULL)
			len++;
		my_env = realloc(environ, sizeof(char *) * (len + 2));
		if (!my_env)
		{
			free (my_env);
			exit (1);
		}
		my_env[len] = new_env;
		my_env[len + 1] = NULL;
		environ = my_env;
	}
}

int	ft_export(char **args, char **env)
{
	char	*env_value;
	int	i;

	i = 1;
	if (args[1] == NULL)
	{
		while (*env)
		{
			printf ("declarxe -x %s\n", *env);
			env++;
		}
		return (0);
	}
	while (args[i])
	{
		env_value = args[i];
		if (ft_strchr(env_value, '=') == NULL || env_value[0] == '=')
		{
			exit(1);
		}
		add_env_var(env_value);
		i++;
	}
	return (0);
}
