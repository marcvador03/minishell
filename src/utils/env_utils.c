/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:00:56 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/26 13:03:53 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (set_status(flush_errors("", -1, 0), err), NULL);
	i = 0;
	while (env != NULL)
	{
		tmp = ft_strjoin(env->varname, "=");
		env_arr[i++] = ft_strjoin(tmp, env->value);
		if (tmp == NULL || env_arr[i - 1] == NULL)
			return (set_status(flush_errors("", 202, 0), err), NULL);
		free(tmp);
		env = env->next;
	}
	env_arr[n] = NULL;
	return (env_arr);
}

