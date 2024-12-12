/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:55:30 by mfleury           #+#    #+#             */
/*   Updated: 2024/12/12 18:20:40 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

/*int	term_settings(t_terms *tcap, char *term_buffer)
{
	tcap->keys_on = tgetstr("ks", &term_buffer);
	if (tcap->keys_on != 0)
		tputs(tcap->keys_on, 1, ft_putchar);
	tcap->keys_off = tgetstr("ke", &term_buffer);
	if (tcap->keys_on == 0 || tcap->keys_off == 0)
		return (-1);
	tcap->up_arrow = tgetstr("ku", &term_buffer);
	tcap->down_arrow = tgetstr("kd", &term_buffer);
	if (tcap->up_arrow == 0 || tcap->down_arrow == 0)
		return (-1);
	tcap->backspace = tgetstr("kb", &term_buffer);
	tcap->del_line = tgetstr("dl", &term_buffer);
	if (tcap->del_line == 0 || tcap->backspace == 0)
		return (-1);
	tcap->set_cursor_begin = tgetstr("cr", &term_buffer);
	if (tcap->set_cursor_begin == 0)
		return (-1);
	return (0);
}*/

void	set_term_settings(t_terms *tcap)
{
	tcap->new_term = tcap->old_term;
	//tcap->new_term.c_iflag &= ~IGNCR;
	//tcap->new_term.c_lflag &= ~ISIG;
	//tcap->new_term.c_iflag = ICRNL;
	//tcap->new_term.c_iflag &= ~IXON;
	//tcap->new_term.c_iflag &= ~IXOFF;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tcap->new_term) == -1)
		exit_minishell_error(NULL, 200);
}

void	unset_term_settings(t_terms *tcap)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tcap->old_term) == -1)
		exit_minishell_error(NULL, 200);
}

void	init_termcaps(t_terms *tcap)
{
	int		success;
	char	term_buffer[2048];
	char	*term_type;

	if (tcgetattr(STDIN_FILENO, &tcap->old_term) == -1)
		exit_minishell_error(NULL, 200);
	term_type = getenv("TERM");
	if (term_type == 0)
		exit_minishell_error(NULL, 200);
	success = tgetent(term_buffer, term_type);
	if (success <= 0)
		exit_minishell_error(NULL, 200);
	/*if (term_settings(tcap, term_buffer) == -1)
		exit_minishell_error(NULL, 200);*/
}
