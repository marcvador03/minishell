/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:31:15 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/09 00:15:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_prompt(t_prompt *t)
{
	char	*res;
	char	*tmp;
	int		len[5];
	
	len[0] = ft_strlen(t->user);
	len[1] = ft_strlen(t->hostname);
	len[2] = ft_strlen(t->cur_path);
	len[3] = ft_strlen(t->status);
	len[4] = len[0] + len[1] + len[2] + len[3];
	len[4] += ft_strlen("@:$() ");
	res = (char *)ft_calloc(sizeof(char), len[4] + 1);
	if (res == NULL)
		return (set_gstatus(202), NULL);
	ft_memcpy(res, t->user, len[0]);
	ft_memcpy(res + len[0], "@", 1);
	ft_memcpy(res + len[0] + 1, t->hostname, len[1]);
	ft_memcpy(res + len[0] + len[1] + 1, ":", 1);
	ft_memcpy(res + len[0] + len[1] + 2, t->cur_path, len[2]);
	tmp = res + len[0] + len[1] + len[2] + 2;
	ft_memcpy(tmp, "(", 1);
	ft_memcpy(tmp + 1, t->status, len[3]);
	ft_memcpy(tmp + len[3] + 1, ")", 1);
	ft_memcpy(tmp + len[3] + 2, "$ ", 2);
	res[len[4]] = '\0';
	return (res);
}

static char	*replace_home_path(char *home)
{
	char	*ex_path;
	char	*res;
	int		len[2];
		
	ex_path = getcwd(NULL, 0);
	if (ex_path == NULL)
		return (set_gstatus(202), NULL);
	if (home == NULL || *home == '\0')
		return (ex_path);
	len[0] = ft_strlen(ex_path);
	len[1] = ft_strlen(home);
	if (len[0] >= len[1] && home[0] == '/' && home[len[1] - 1] != '/')
	{
		if (ft_strncmp(ex_path, home, ft_strlen(home)) == 0)
		{
			res = (char *)ft_calloc(sizeof(char), len[0] - len[1] + 2);
			if (res == NULL)
				return (set_gstatus(202), ex_path);
			res[0] = '~';
			ft_memcpy(res + 1, ex_path + len[1], len[0] - len[1]);
			res[len[0] - len[1] + 1] = '\0';
			return (free_s(ex_path), res);
		}
	}
	return (ex_path);
}

static char	*get_user(t_env *env)
{
	char		*user;
	int			slot;
	int			fd;
	struct utmp ut;
	int 		len;

	user = sh_getenv(env, "USER");
	if (user != NULL)
		return (user);
	slot = ttyslot();
	user = NULL;
	len = sizeof(struct utmp);
	fd = open(UTMP_FILE, O_RDONLY);
	if (fd == -1)
		return ("unk");
	while (read(fd, &ut, len) == len)
	{
		if (ft_atoi(ut.ut_id + 1) == slot) 
			user = ut.ut_user;
		printf("%s\n", ut.ut_id + 1);
	}
	return (user);
}

static char	*get_hostname()
{
	int		fd;
	char	buf[65];
	char	*host;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (read(fd, &buf, 64) > 1)
	{
		buf[64] = '\0';
		host = ft_strtrim(buf, "\n");
		if (host == NULL)
			return (set_gstatus(202), NULL);
	}	return ((host));
	return (NULL);
}
char	*create_prompt(t_env *env)
{
	t_prompt	t;

	t.user = get_user(env);
	t.hostname = get_hostname();
	t.home = sh_getenv(env, "HOME");
	t.cur_path = replace_home_path(t.home);
	t.status = ft_itoa(g_status);
	t.prompt = join_prompt(&t);
	free_s(t.status);
	free_s(t.cur_path);
	free_s(t.hostname);
	return (t.prompt);
}
/*
*/

