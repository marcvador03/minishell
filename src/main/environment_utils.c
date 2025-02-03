/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:53:05 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/03 21:15:26 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*fill_default_session(t_env *env)
{
	char	*level;
	int		x;
	t_env	*env2;

	env2 = env;
	if (sh_getenv(env2, "SHLVL", 1) == NULL)
		env2 = sh_addenv(env2, ft_strdup("SHLVL"), ft_strdup("1"));
	else
	{
		x = ft_atoi(sh_getenv(env2, "SHLVL", 0));
		level = ft_itoa(x + 1);
		sh_updateenv(env2, ft_strdup("SHLVL"), level);
	}
	if (sh_getenv(env2, "TERM", 1) == NULL)
		env = sh_addenv(env2, ft_strdup("TERM"), ft_strdup("xterm-256color"));
	if (sh_getenv(env2, "_", 1) == NULL)
		env = sh_addenv(env2, ft_strdup("_"), ft_strdup("./minishell"));
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
			return (flush_errors("", 202, 0), fill_default_session(NULL));
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

int	env_size(t_env *lst)
{
	int		cnt;
	t_env	*tmp;

	cnt = 0;
	tmp = lst;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		cnt++;
	}
	return (cnt);
}

char	**get_env_array(t_env *env, int *err)
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
		return (set(flush_errors("", -1, 0), err), NULL);
	i = 0;
	while (env != NULL)
	{
		tmp = ft_strjoin(env->varname, "=");
		env_arr[i++] = ft_strjoin(tmp, env->value);
		if (tmp == NULL || env_arr[i - 1] == NULL)
			return (set(flush_errors("", 202, 0), err), NULL);
		free(tmp);
		env = env->next;
	}
	env_arr[n] = NULL;
	return (env_arr);
}
