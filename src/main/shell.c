/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/10 22:48:56 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
int		count_tokens(char *line);
int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int i, int level);
int		check_input(char *line);
int		init_data_brackets(t_shell *tmp, int *a, int *b);
char	*create_prompt(t_env *env);

static int	fill_sh_init(t_shell *tmp, t_terms *tcap, t_env *env, int (*x)[2])
{
	tmp->s_line = expand_env(tmp->s_line, env, 1);
	if (tmp->s_line == NULL)
		return (set_gstatus(202), 202);
	tmp->tcap = tcap;
	tmp->bracket[0] = *x[0] + tmp->bracket[0] - (*x)[1];
	(*x)[0] = tmp->bracket[0];
	(*x)[1] = tmp->bracket[1];
	return (0);
}

static t_shell	*fill_sh(t_shell *sh, char *line, t_terms *tcap, t_env *env)
{
	int		i;
	t_shell	*tmp;
	int		s_bracket[2];
	char	*t_line;
	int		n;

	i = 0;
	n = count_tokens(line);
	s_bracket[0] = 0;
	s_bracket[1] = 0;
	while (i++ < n)
	{
		t_line = line + sh_skip(line, ' ');
		if (sh == NULL)
			tmp = sh_lstnew(t_line, env);
		else
			tmp = sh_lstadd_back(&sh, t_line, env);
		if (tmp == NULL || fill_sh_init(tmp, tcap, env, &s_bracket) != 0)
			return (NULL);
		sh = tmp;
	}
	if (sh->bracket[1] != sh->bracket[0])
		return (set_gstatus(206), NULL);
	return (sh->head);
}

static char	*get_input(t_env *env)
{
	char	*line;
	char	*line2;
	char	*prompt;

	prompt = create_prompt(env);
	if (prompt == NULL)
		return (NULL);
	line = readline(prompt);
	if (line == NULL)
		return (free_s(prompt), NULL);
	else if (ft_strlen(line) == 0 && line[0] == '\0')
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input(env));
	else if (check_input(line) != 0)
		return (free_s(prompt), free_s(line), set_gstatus(0), get_input(env));
	add_history(line);
	line2 = ft_strjoin("&&", line);
	return (free_s(prompt), free_s(line), line2);
}

int	start_shell(t_env *env, t_terms *tcap)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env);
	if (line == NULL)
		exit_minishell_error(sh, g_status, env);
	if (check_open_quotes(line) == -1)
		return (free_s((void *)line), set_gstatus(201), -1);
	sh = fill_sh(sh, line, tcap, env);
	if (sh == NULL)
		return (free_s((void *)line), -1);
	free_s((void *)line);
	head = sh->head;
	g_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	execute_tokens(sh, 0, 0);
	//	return (free_sh(head), 1);
	return (free_sh(head), 0);
}
