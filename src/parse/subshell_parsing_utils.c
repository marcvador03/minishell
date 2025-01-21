/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_parsing_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 21:50:58 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/21 22:06:24 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
	// function get_shell_redirections
		/*
		if (none_of_char(s_line[q->i], "<,>") == TRUE)
			return (flush_errors("", 250, ""), -1);
		q->k = 0;
		q->prev_pos = q->i;
		while (one_of_char(s_line[q->i], "<,>") == TRUE)
		{
			if (q->k >= 2)
				return (flush_errors("", 250, ""), -1);
			if (s_line[q->i] == '>' && s_line[q->i] == '<')
				return (flush_errors("", 250, ""), -1);
			else if (s_line[q->i] == '<' && s_line[q->i] == '>')
				return (flush_errors("", 250, ""), -1);
			q->k++;
		}
		q->i += sh_skip(s_line, ' ');
		if (s_line[q->i] == '\0')
			return (flush_errors("", 250, ""), -1);
		sh->s_rd[q->j] = ft_substr(s_line, q->prev_pos, q->i - q->prev_pos);
		if (sh->s_rd[q->j] == NULL)
			return (flush_errors("", 202, ""), -1);
		q->prev_pos = q->i;
		while (s_line[q->i] != '\0' && one_of_char(s_line[q->j], "<,>") == TRUE)
			q->i++;
		sh->s_redirs[q->j] = ft_substr(s_line, q->prev_pos, q->i - q->prev_pos);
		if (sh->s_rd[q->j++] == NULL)
			return (flush_errors("", 202, ""), -1);
		if (s_line[q->i] == '\0')
			return (0);
		else if (one_of_char(s_line[q->j], "<,>") == TRUE)
			return (call same function, 0);
	}*/
