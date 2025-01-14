/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varenv_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:48:32 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/14 13:30:00 by mfleury          ###   ########.fr       */
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

char	*resize_line(char *line, char *out, char *in, int *i)
{
	char	*res;
	char	*tmp[3];
	int		len[4];

	len[0] = ft_strlen(in);
	len[1] = ft_strlen(line);
	len[2] = ft_strlen(out);
	tmp[0] = ft_substr(line, 0, *i);
	tmp[1] = ft_substr(line, *i + len[0] + 1, len[1]);
	tmp[2] = ft_strjoin(tmp[0], out);
	if (tmp[2] == NULL)
		return (NULL);
	*i = max(0, ft_strlen(tmp[2]) - 1);
	res = ft_strjoin(tmp[2], tmp[1]);
	if (res == NULL)
		return (NULL);
	free_s(out);
	free_s(line);
	return (free_s(tmp[0]), free_s(tmp[1]), free_s(tmp[2]), res);
}

int	ft_isalnum_plus(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	else if (c >= 48 && c <= 57)
		return (1);
	else if (c == '@' || c == '_')
		return (1);
	else
		return (0);
}
static int	get_full_path_check(char *arg0, char **cmd_out)
{
	if (access(arg0, X_OK) == 0)
	{
		*cmd_out = ft_strdup(arg0);
		return (-1);
	}
	else if(ft_strncmp(arg0, "", max(ft_strlen(arg0), 1)) == 0)
	{
		*cmd_out = ft_strdup("");
		return (-1);
	}
	return (0);
}

char	*get_full_path(char *arg0, t_env *env)
{
	char	*cmd_in;
	char	*cmd_out;
	char	**paths;
	int		i;

	if (get_full_path_check(arg0, &cmd_out) == -1)
		return (cmd_out);
	paths = ft_split(sh_getenv(env, "PATH"), ':');
	if (paths == NULL)
		return (free_s(paths), set_gstatus(125), NULL);
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
