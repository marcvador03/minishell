/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:54:49 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/14 09:52:36 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap(char **str1, char **str2);
int		export_count(char **env);
int		check_export_var(char *str);
char	*export_env_value(char *args, int n);
int		check_option(char *str, char *cmd);

static char	**sort_array(char **s_env, int o, int n)
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
				swap(&s_env[++i], &s_env[j]);
			j++;
		}
		swap(&s_env[++i], &s_env[n]);
		s_env = sort_array(s_env, o, i - 1);
		s_env = sort_array(s_env, i + 1, n);
	}
	return (s_env);
}

static void	print_sorted_loop(char **s_env, int i)
{
	int	j;

	j = 0;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	while (s_env[i][j] != '=' && s_env[i][j] != '\0')
		ft_putchar_fd(s_env[i][j++], STDOUT_FILENO);
	if (s_env[i][j] == '\0' || (s_env[i][j] == '=' && s_env[i][j + 1] == '\0'))
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd(&s_env[i][++j], STDOUT_FILENO);
	ft_putstr_fd("\"\n", STDOUT_FILENO);
}

static int	print_sorted(t_env *env, int n)
{
	int		i;
	char	**s_env;

	s_env = get_env_array(env);
	if (s_env == NULL)
		return (202);
	s_env = sort_array(s_env, 0, n - 1);
	i = 0;
	while (s_env[i] != NULL)
	{
		if (!(s_env[i][0] == '_' && s_env[i][1] == '='))
		{
			if (ft_strchr(s_env[i], '=') != 0)
				print_sorted_loop(s_env, i);
			else
				printf ("declarxe -x %s\n", s_env[i]);
		}
		i++;
	}
	return (free_d(s_env), 0);
}

static void	ft_export_loop(char *arg, t_env *env)
{
	char	*var_name;
	char	*env_value;
	int		n;

	n = sh_strpos(arg, "=");
	var_name = ft_substr(arg, 0, n);
	if (arg[n] != '\0')
		env_value = ft_strdup(export_env_value(arg, n));
	else
		env_value = NULL;
	sh_updateenv(env, var_name, env_value);
}

int	ft_export(char **args, t_env *env)
{
	int		i;
	int		err;

	i = 1;
	err = 0;
	if (args[1] == NULL)
		return (print_sorted(env, env_size(env)));
	err = check_option(args[1], "export");
	if (err == 2)
		return (err);
	while (args[i])
	{
		if (check_export_var(args[i]) == 0)
			ft_export_loop(args[i], env);
		else
			err = 1;
		i++;
	}
	return (err);
}
