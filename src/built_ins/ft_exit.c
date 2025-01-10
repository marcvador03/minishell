/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:34:22 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/10 13:49:20 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

static int	check_numeric(char *args, int i)
{
	if (ft_isdigit(args[i]) == 1)
		return (set_gstatus(2), 2);
	if (i == 0 && (args[i] == '-' || args[i] == '+'))
		return (0);
	if (args[i] == ' ')
		return (0);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putendl_fd(E_012, STDERR_FILENO);
	return (set_gstatus(2), -1);
}

static int	check_overflow(char *str)
{
	char	*tmp;
	t_ll	n;

	n = ll_atoi(str);
	tmp = ll_itoa(n);
	if (ft_strncmp(str, tmp, ft_strlen(tmp)) != 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putendl_fd(E_012, STDERR_FILENO);
		return (set_gstatus(2), free_s(tmp), -1);
	}
	return (free_s(tmp), 0);
}

int	ft_exit(t_pipe *p, char **args, t_env *env)
{
	int	i;
	
	i = 0;
	if (args[1] != NULL)
	{
		while (args[1][i] != 0)
			if (check_numeric(args[1], i++) == -1)
				return (exit_minishell(p, env), 0);
	}
	if (args[1] != NULL)
	{
		if (check_overflow(args[1]) == -1)
			return (exit_minishell(p, env), 0);
		else if (args[1] != NULL && args[2] != NULL)
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putendl_fd(E_007, STDERR_FILENO);
			return (1);
		}
		else
			p->p_status = ft_atoi(args[1]);
	}
	exit_minishell(p, env);
	return (p->p_status);
}
