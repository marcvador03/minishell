/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/06/20 12:37:44 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		execute_tokens(t_shell *sh, int status, t_env **env);
char	*create_prompt(t_env *env);
t_shell	*sh_lstnew(t_terms *tcap, t_env *env, int *l_status);
t_shell	*parse_sh(t_shell *sh, char *line, int *pos, int *l_status);

static int	check_brackets(char *line)
{
	int	i;
	int	bracket;

	i = 0;
	bracket = 0;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
		{
			i += sh_jump_to(line + i, line[i]);
			if (line[i] == '\0' && bracket != 0)
				return (flush_errors("", 210, ')'), -1);
			else if (line[i] == '\0')
				return (0);
		}
		if (line[i] == '(')
			bracket++;
		else if (line[i] == ')')
			bracket--;
		i++;
	}
	if (bracket != 0)
		return (flush_errors("", 210, ')'), -1);
	return (0);
}

static int	check_forbidden_c(char *line, int *l_status)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		if ((line[i] > 0 && line[i] <= 31) || line[i] == 92)
			return (set(flush_errors("", 210, line[i]), l_status), -1);
		i++;
	}
	i = 0;
	while (line[i] != '\0')
	{
		while (line[i] == 34 || line[i] == 39)
		{
			i += sh_jump_to(line + i, line[i]);
			if (line[i] == '\0')
				return (0);
		}
		if (line[i] == ';' || line[i] == 42)
			return (set(flush_errors("", 210, line[i]), l_status), -1);
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
	if (check_brackets(str) == -1)
		return (-1);
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
	line = sh_trim_spaces(line);
	if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	if (g_status == 130)
		*l_status = 130;
	g_status = 0;
	add_history(line);
	if (check_forbidden_c(line, l_status) == -1)
		return (free_s(prompt), free_s(line), get_input(env, tcap, l_status));
	return (free_s(prompt), line);
}

int	start_shell(t_env **env, t_terms *tcap, int *l_status)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;
	int		i;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(*env, tcap, l_status);
	if (line == NULL)
		exit_minishell_error(sh, 200, *env);
	if (check_open_quotes(line) == -1)
		return (set(2, l_status), free_s(line), 2);
	sh = sh_lstnew(tcap, *env, l_status);
	if (sh == NULL)
		return (set(flush_errors("", 202, 0), l_status), 0);
	i = 0;
	sh = parse_sh(sh, line, &i, l_status);
	if (sh == NULL)
		return (free_s(line), 0);
	free_s(line);
	head = sh->head;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	*l_status = execute_tokens(head, 0, env);
	return (free_sh(head), 0);
}
