/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 09:05:40 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/08 00:49:08 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*sh_getenv(t_env *env, char *str)
{
	int		len[2];

	if (env == NULL || str == NULL)
		return (NULL);
	len[0] = ft_strlen(str);
	while (env != NULL)
	{
		len[1] = ft_strlen(env->varname);
		if (ft_strncmp(env->varname, str, max(len[0], len[1])) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	sh_addenv(t_env *env, char *var_name, char *value)
{
	t_env	*ptr;

	if (var_name == NULL)
		return ;
	ptr = (t_env *)ft_calloc(sizeof(t_env), 1);
	if (ptr == NULL)
		return (set_gstatus(202));
	ptr->head = ptr;
	ptr->next = NULL;
	ptr->varname = var_name;
	ptr->value = value;
	if (env == NULL)
		return ;
	while (env->next != NULL)
		env = env->next;
	ptr->head = env->head;
	env->next = ptr;
	return ;
}

/*char	**sh_add_env(char ***env, char *var_name, char *new_value)
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
}*/
void	sh_updateenv(t_env *env, char *var_name, char *new_value)
{
	int		len[2];
	char	*tmp;

	if (var_name == NULL)
		return ;
	len[0] = ft_strlen(var_name);
	while (env != NULL)
	{
		len[1] = ft_strlen(env->varname);
		if (ft_strncmp(env->varname, var_name, max(len[0], len[1])) == 0)
		{
			tmp = env->value;
			env->value = new_value;
			return (free_s(tmp), free_s(var_name));
		}
		env = env->next;
	}
	sh_addenv(env, var_name, new_value);
	return;
}
/*char	**sh_update_env(char ***env, char *var_name, char *new_value)
{
	int		i;
	int		len[2];
	char	*entry;
	char	*tmp;

	if (*env == NULL || var_name == NULL)
		return (NULL);
	len[0] = ft_strlen(var_name);
	entry = NULL;
	i = 0;
	while ((*env)[i] != NULL)
	{
		len[1] = ft_strlen((*env)[i]);
		if (ft_strncmp((*env)[i], var_name, max(len[0], len[1])) == 0)
		{
			if (new_value == NULL)
				return (*env);
			entry = create_entry(var_name, new_value);
			tmp = (*env)[i];
			(*env)[i] = entry;
			return (free_s(tmp), *env);
		}
		i++;
	}
	return (free_s(entry), sh_add_env(env, var_name, new_value));
}*/

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
				update_head (env->next, env->next);
			else
				tmp->next = env->next;
			free_s(env->varname);
			free_s(env->value);
			free_s(env);
			return (tmp->next);
		}
		tmp = env;
		env = env->next;
	}
	return (env->head);
}	

/*char	**sh_del_env(char ***env, char *str)
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
}*/
