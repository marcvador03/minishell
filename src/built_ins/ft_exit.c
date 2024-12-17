/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:34:22 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/17 21:56:38 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_errors(char *args, int i)
{
	if (ft_isdigit(args[i]) == 1)
		return (0);
	if (i == 0 && (args[i] == '-' || args[i] == '+'))
		return (0);
	if (args[i] == ' ')
		return (0);
	return (-1);
}

static int	check_overflow(char *str)
{
	char	*tmp;
	int		n;

	n = ft_atoi(str);
	tmp = ft_itoa(n);
	if (ft_strncmp(str, tmp, ft_strlen(tmp)) != 0)
		return (free_s(tmp), -1);
	return (free_s(tmp), 0);
}

void	ft_exit(t_shell *sh, char **args, char **env)
{
	int	i;

	i = 0;
	if (args[1] != NULL && args[2] != NULL)
		return (flush_errors("exit", 7));
	else if (args[1] != NULL)
	{
		while (args[1][i] != 0)
		{
			if (check_errors(args[1], i) == -1)
				return (flush_errors("exit", 12));
			i++;
		}
	}
	g_status = 0;
	if (args[1] != NULL)
	{
		if (check_overflow(args[1]) == -1)
			return (flush_errors("exit", 12));
		else
			g_status = ft_atoi(args[1]);
	}
	exit_minishell(sh, env);
	return ;
}
