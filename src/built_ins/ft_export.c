/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:54:49 by pmorello          #+#    #+#             */
/*   Updated: 2024/12/15 00:04:59 by mfleury          ###   ########.fr       */
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

static void	swap(char **str1, char **str2)
{
	char	*tmp;
	
	tmp = *str1;
	*str1 = *str2;
	*str2 = tmp;
}

static void	sort_array(char **s_env, int o, int n)
{
	char	*pivot;
	int		i;
	int		j;
	int		len[2];

	if (o < n)
	{
		pivot = s_env[n];
		len[0] = ft_strlen(pivot);
		i = o - 1;
		j = o;
		while (j < n)
		{
			len[1] = ft_strlen(s_env[j]);
			if (ft_strncmp(pivot, s_env[j], max(len[0], len[1])) > 0)
				swap(&s_env[i++], &s_env[j]);
			j++;
		}
		swap(&s_env[++i], &s_env[n]);
		sort_array(s_env, o, i - 1);
		sort_array(s_env, i + 1, n);
	}
}

static int	print_sorted(char **env)
{
	int		i;
	int		j;
	int		n;
	char	**s_env;

	s_env = env;
	n = 0;
	while (s_env[n] != NULL)
		n++;
	sort_array(s_env, 0, n - 1);
	i = 0;
	while (s_env[i] != NULL)
	{
		if (ft_strchr(s_env[i], '=') != 0)
		{
			j = 0;
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			while (s_env[i][j] != '=')
				ft_putchar_fd(s_env[i][j++], STDOUT_FILENO);
		}
		else
			printf ("declarxe -x %s\n", s_env[i]);
		i++;
	}
	return (0);	
}

int	ft_export(char **args, char **env)
{
	char	*env_value;
	int	i;

	i = 1;
	if (args[1] == NULL)
		return (print_sorted(env));
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
