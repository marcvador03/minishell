/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:53:05 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/11 19:11:46 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*fill_default_session(t_env *env)
{
	char	*level;
	int		x;
	t_env	*env2;

	env2 = env;
	if (sh_getenv(env2, "SHLVL") == NULL)
		env2 = sh_addenv(env2, ft_strdup("SHLVL"), ft_strdup("1"));
	else
	{
		x = ft_atoi(sh_getenv(env2, "SHLVL"));
		level = ft_itoa(x + 1);
		sh_updateenv(env2, ft_strdup("SHLVL"), level);
	}
	flush_errors("", g_status);
	if (sh_getenv(env2, "TERM") == NULL)
		env = sh_addenv(env2, ft_strdup("TERM"), ft_strdup("xterm-256color"));
	if (sh_getenv(env2, "_") == NULL)
		env = sh_addenv(env2, ft_strdup("_"), ft_strdup("./minishell"));
	flush_errors("", g_status);
	return (env2);
}

static void	fill_content(t_env **ptr, char *envp[], int i)
{
	int	n;

	n = sh_strpos(envp[i], "=");
	(*ptr)->varname = ft_substr(envp[i], 0, n);
	if (envp[i][n] == '\0')
		(*ptr)->value = NULL;
	else
		(*ptr)->value = ft_substr(envp[i], n + 1, ft_strlen(envp[i]));
	(*ptr)->head = *ptr;
	(*ptr)->next = NULL;
	return ;
}

t_env	*fill_env(char *envp[])
{
	t_env	*env;
	t_env	*ptr;
	int		i;

	if (envp == NULL || envp[0] == NULL)
		return (fill_default_session(NULL));
	env = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		ptr = (t_env *)ft_calloc(sizeof(t_env), 1);
		if (ptr == NULL)
			return (set_gstatus(202), fill_default_session(NULL));
		fill_content(&ptr, envp, i);
		if (env != NULL)
		{
			ptr->head = env->head;
			env->next = ptr;
		}
		env = ptr;
		i++;
	}
	env = fill_default_session(env->head);
	return (env->head);
}

char	**get_env_array(t_env *env)
{
	int		n;
	int		i;
	char	**env_arr;
	char	*tmp;

	if (env == NULL)
		return (NULL);
	env = env->head;
	n = env_size(env);
	env_arr = (char **)ft_calloc(sizeof(char *), n + 1);
	if (env_arr == NULL)
		return (set_gstatus(202), NULL);
	i = 0;
	while (env != NULL)
	{
		tmp = ft_strjoin(env->varname, "=");
		env_arr[i++] = ft_strjoin(tmp, env->value);
		free(tmp);
		env = env->next;
	}
	env_arr[n] = NULL;
	return (env_arr);
}

char	*get_full_path(char *arg0, t_env *env)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (access(arg0, X_OK) == 0)
		return (ft_strdup(arg0));
	paths = ft_split(sh_getenv(env, "PATH"), ':');
	if (paths == NULL)
		return (free_s(paths), set_gstatus(2), NULL);
	cmd_in = ft_strjoin("/", arg0);
	if (cmd_in == NULL)
		return (free_d(paths), set_gstatus(202), NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_out = ft_strjoin(paths[i++], cmd_in);
		if (cmd_out == NULL)
			return (free_d(paths), free_s(cmd_in), set_gstatus(202), NULL);
		if (access(cmd_out, X_OK) == 0)
			return (free_d(paths), free_s(cmd_in), cmd_out);
		free_s(cmd_out);
	}
	return (free_d(paths), free_s(cmd_in), set_gstatus(127), NULL);
}
