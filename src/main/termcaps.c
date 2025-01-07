/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 22:55:30 by mfleury           #+#    #+#             */
/*   Updated: 2025/01/07 22:42:27 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

void	set_term_settings(t_terms *tcap, t_env *env)
{
	tcap->new_term = tcap->old_term;
	tcap->new_term.c_iflag &= ~(IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK);
	tcap->new_term.c_iflag &= ~(ISTRIP | INLCR | IGNCR | IUCLC | IXANY);
	tcap->new_term.c_iflag &= ~(IMAXBEL | IXOFF);
	tcap->new_term.c_iflag |= (ICRNL | IXON);
	tcap->new_term.c_oflag &= ~(OLCUC | OCRNL | ONLRET | OFILL | OFDEL);
	tcap->new_term.c_oflag |= (OPOST | ONLCR);
	tcap->new_term.c_cflag &= ~(CSTOPB | PARENB | PARODD | HUPCL | CLOCAL);
	tcap->new_term.c_cflag |= (CREAD);
	tcap->new_term.c_lflag &= ~(ECHONL | NOFLSH | TOSTOP);
	tcap->new_term.c_lflag |= (ISIG | ICANON | ECHO | ECHOE | ECHOK | IEXTEN);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tcap->new_term) == -1)
		exit_minishell_error(NULL, 200, env);
}

void	unset_term_settings(t_terms *tcap, t_env *env)
{
	if (tcap == NULL)
		return ;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tcap->old_term) == -1)
		exit_minishell_error(NULL, 200, env);
}

void	init_termcaps(t_terms *tcap, t_env *env)
{
	int		success;
	char	term_buffer[2048];
	char	*term_type;

	if (tcgetattr(STDIN_FILENO, &tcap->old_term) == -1)
		exit_minishell_error(NULL, 200, env);
	term_type = sh_getenv(env, "TERM");
	if (term_type == 0 || term_type == NULL)
		exit_minishell_error(NULL, 200, env);
	success = tgetent(term_buffer, term_type);
	if (success <= 0)
		exit_minishell_error(NULL, 200, env);
}
