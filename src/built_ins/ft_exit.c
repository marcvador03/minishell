/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:34:22 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/11 19:19:05 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "error_minishell.h"

static int	check_numeric(char *args, int i, int *status)
{
	if (ft_isdigit(args[i]) == 1)
		return (0);
	if (i == 0 && (args[i] == '-' || args[i] == '+'))
		return (0);
	if (args[i] == ' ')
		return (0);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putendl_fd(E_012, STDERR_FILENO);
	*status = 2;
	return (*status);
}

static int	check_overflow(char *str, int *status)
{
	char	*tmp;
	t_ll	n;

	n = ll_atoi(str);
	tmp = ll_itoa(n);
	if (ft_strncmp(str, tmp, ft_strlen(tmp)) != 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putendl_fd(E_012, STDERR_FILENO);
		*status = 2;
		return (free_s(tmp), *status);
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
			if (check_numeric(args[1], i++, &p->p_status) == 2)
				return (exit_minishell(p, env), p->p_status);
	}
	if (args[1] != NULL)
	{
		if (check_overflow(args[1], &p->p_status) == 2)
			return (exit_minishell(p, env), p->p_status);
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
