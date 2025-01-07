/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 09:54:49 by pmorello          #+#    #+#             */
/*   Updated: 2025/01/07 16:50:18 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap(char **str1, char **str2);
int		export_count(char **env);
int		check_export_var(char *str);
char	*export_env_value(char *args, int n);

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

static int	print_sorted(char **env, int n)
{
	int		i;
	char	**s_env;

	s_env = fill_env(env);
	if (s_env == NULL)
		return (202);
	s_env = sort_array(s_env, 0, n - 1);
	i = 0;
	while (s_env[i] != NULL)
	{
		if (ft_strchr(s_env[i], '=') != 0)
			print_sorted_loop(s_env, i);
		else
			printf ("declarxe -x %s\n", s_env[i]);
		i++;
	}
	return (free_d(s_env), 0);
}

int	ft_export(char **args, char ***env)
{
	char	*var_name;
	char	*env_value;
	int		i;
	int		n;

	i = 1;
	if (args[1] == NULL)
		return (print_sorted(*env, export_count(*env)));
	while (args[i])
	{
		if (check_export_var(args[i]) != -1)
		{
			n = sh_strpos(args[i], "=");
			var_name = ft_substr(args[i], 0, n);
			if (args[i][n] != '\0')
				env_value = export_env_value(args[i], n);
			else
				env_value = NULL;
			*env = sh_update_env(env, var_name, env_value);
			if (env_value != NULL)
				free_s(var_name);
		}
		i++;
	}
	return (0);
}
