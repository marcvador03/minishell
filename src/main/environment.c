/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 09:05:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/02/05 19:11:49 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sh_getenv(t_env *env, char *str, int flag)
{
	int		len[2];

	if (env == NULL || str == NULL)
		return (NULL);
	len[0] = ft_strlen(str);
	while (env != NULL)
	{
		len[1] = ft_strlen(env->varname);
		if (ft_strncmp(env->varname, str, max(len[0], len[1])) == 0)
		{
			if (flag == 1)
				return (env->varname);
			else
				return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}

t_env	*sh_addenv(t_env *env, char *var_name, char *value)
{
	t_env	*ptr;

	if (var_name == NULL)
		return (env);
	ptr = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (ptr == NULL)
		return (flush_errors("", -1, 0), env);
	ptr->head = ptr;
	ptr->next = NULL;
	ptr->varname = var_name;
	ptr->value = value;
	if (env == NULL)
		return (ptr);
	while (env->next != NULL)
		env = env->next;
	ptr->head = env->head;
	env->next = ptr;
	return (env->head);
}

t_env	*sh_updateenv(t_env *env, char *var_name, char *new_value)
{
	int		len[2];
	t_env	*head;

	if (var_name == NULL)
		return (env);
	head = NULL;
	if (env != NULL)
		head = env->head;
	len[0] = ft_strlen(var_name);
	while (env != NULL)
	{
		len[1] = ft_strlen(env->varname);
		if (ft_strncmp(env->varname, var_name, max(len[0], len[1])) == 0)
		{
			if (new_value != NULL)
			{
				free_s(env->value);
				env->value = new_value;
			}
			return (free_s(var_name), env->head);
		}
		env = env->next;
	}
	return (sh_addenv(head, var_name, new_value));
}

static void	update_head(t_env *env, t_env *head)
{
	while (env != NULL)
	{
		env->head = head;
		env = env->next;
	}
	return ;
}

t_env	*sh_delenv(t_env *env, char *var_name)
{
	int		len[2];
	t_env	*tmp;

	if (env == NULL || var_name == NULL)
		return (NULL);
	len[0] = ft_strlen(var_name);
	tmp = env;
	while (env != NULL)
	{
		len[1] = ft_strlen(env->varname);
		if (ft_strncmp(env->varname, var_name, max(len[0], len[1])) == 0)
		{
			if (env == env->head)
				update_head (env, env->next);
			else
				tmp->next = env->next;
			free_s(env->varname);
			free_s(env->value);
			free_s(env);
			return (tmp->head);
		}
		tmp = env;
		env = env->next;
	}
	return (env->head);
}
