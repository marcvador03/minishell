/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/25 20:16:30 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		execute_tokens(t_shell *sh, int status);
char	*create_prompt(t_env *env);
t_shell	*sh_lstnew(t_terms *tcap, t_env *env, int *l_status);
t_shell	*parse_sh(t_shell *sh, char *line, int *pos, int *l_status);

static int	check_forbidden_c(char *line)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		if (line[i] == ';' || line[i] == 92 || line[i] == 10 || line[i] == 42)
		{
			flush_errors("", 210, line[i]);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	check_open_quotes(char *str)
{
	int	i;
	int	x;

	i = 0;
	while (str[i] != 0)
	{
		if (str[i] == 34 || str[i] == 39)
		{
			if (str[i + 1] == '\0')
				return (flush_errors("", 210, str[i]), -1);
			x = sh_jump_to(str + i, str[i]);
			if (str[x + i - 1] != str[i])
				return (flush_errors("", 210, str[i]), -1);
			else
				i += x;
		}
		else
			i++;
	}
	return (0);
}

static char	*get_input(t_env *env, t_terms *tcap, int *l_status)
{
	char	*line;
	char	*prompt;

	prompt = create_prompt(env);
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
	{
		free_s(prompt);
		unset_term_settings(tcap, env);
		exit_minishell(NULL, env);
	}
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	if (g_status == 130)
		*l_status = 130;
	g_status = 0;
	add_history(line);
	if (check_forbidden_c(line) == -1)
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	return (free_s(prompt), line);
}

int	start_shell(t_env *env, t_terms *tcap, int *l_status)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;
	int		i;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env, tcap, l_status);
	if (line == NULL)
		exit_minishell_error(sh, 200, env);
	if (check_open_quotes(line) == -1)
		return (free_s(line), 2);
	sh = sh_lstnew(tcap, env, l_status);
	if (sh == NULL)
		return (set_status(flush_errors("", 202, 0), l_status), 0);
	i = 0;
	sh = parse_sh(sh, line, &i, l_status);
	if (sh == NULL)
		return (free_s(line), 0);
	free_s(line);
	head = sh->head;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	*l_status = execute_tokens(head, 0);
	return (free_sh(head), 0);
}
