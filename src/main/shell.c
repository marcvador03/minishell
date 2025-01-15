/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:12:52 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/15 20:34:32 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_tk(char *line);
int		count_tokens(char *line);
int		check_open_quotes(char *str);
int		execute_tokens(t_shell *sh, int level);
char	*create_prompt(t_env *env);

static int	fill_sh_init(t_shell *tmp, t_terms *tcap, int (*x)[2]/*, int l_status*/)
{
	//tmp->s_line = expand_env(tmp->env, tmp->s_line, 1, l_status);
	if (tmp->s_line == NULL)
		return (set_gstatus(202), 202);
	tmp->tcap = tcap;
	tmp->bracket[0] = *x[0] + tmp->bracket[0] - (*x)[1];
	(*x)[0] = tmp->bracket[0];
	(*x)[1] = tmp->bracket[1];
	return (0);
}

static t_shell	*fill_sh_loop(t_shell *sh, t_env *env, char *line)
{
	char	*t_line;
	
	t_line = line + sh_skip(line, ' ');
	if (sh == NULL)
		sh = sh_lstnew(t_line, env);
	else
		sh = sh_lstadd_back(&sh, t_line, env);
	if (sh == NULL)
		return (NULL);
	return (sh);
}

static t_shell	*fill_sh(char *line, t_terms *tcap, t_env *env/*, int l_status*/)
{
	int		i;
	int		s_bracket[2];
	int		n;
	t_shell	*sh;

	i = 0;
	sh = NULL;
	n = count_tokens(line);
	if (n == -1)
		return (set_gstatus(204), NULL);
	s_bracket[0] = 0;
	s_bracket[1] = 0;
	while (i < n)
	{
		sh = fill_sh_loop(sh, env, line);
		if (sh == NULL || fill_sh_init(sh, tcap, &s_bracket/*, l_status*/) != 0)
			return (NULL);
		i++;
	}
	if (sh->bracket[1] != sh->bracket[0])
		return (free_sh(sh->head), set_gstatus(206), NULL);
	return (sh->head);
}

static char	*get_input(t_env *env, t_terms *tcap)
{
	char	*line;
	char	*line2;
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
		return (free_s(prompt), free_s(line), get_input(env, tcap));
	add_history(line);
	line2 = ft_strjoin("&&", line);
	if (line2 == NULL)
		return (set_gstatus(202), free_s(line), free_s(prompt), NULL);
	return (free_s(prompt), free_s(line), line2);
}

int	start_shell(t_env *env, t_terms *tcap)
{
	char	*line;
	t_shell	*sh;
	t_shell	*head;

	sh = NULL;
	init_signal(1, 0);
	line = get_input(env, tcap);
	if (line == NULL)
		exit_minishell_error(sh, 200, env);
	if (check_open_quotes(line) == -1)
		return (free_s(line), flush_errors("", 201), 0);
	sh = fill_sh(line, tcap, env/*, g_status*/);
	if (sh == NULL)
		return (free_s(line), flush_errors("", g_status), 0);
	free_s(line);
	head = sh->head;
	g_status = 0;
	if (sh_check_empty(sh->s_line) == -1)
		return (free_sh(head), 0);
	g_status = execute_tokens(sh, 0);
	return (free_sh(head), 0);
}
