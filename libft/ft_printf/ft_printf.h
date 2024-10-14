/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:20:50 by mfleury           #+#    #+#             */
/*   Updated: 2024/08/20 11:49:50 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdlib.h>
# include <stdarg.h>
# include "../libft.h"
# define TRUE 1
# define FALSE 0
# ifdef __unix__
#  define PTRNUL "(nil)"
# elif __APPLE__
#  define PTRNUL "0x0"
# else
#  define PTRNUL "(nil)"
# endif

typedef unsigned long long	t_ull;
typedef unsigned int		t_uint;
static const char			g_specs[9] = \
							{'c', 's', 'd', 'i', 'p', 'x', 'X', 'u', '%'};	
int		ft_printf(const char *s, ...);
void	*ft_printf_c(int n, int *cnt);
char	*ft_printf_s(char *str);
char	*ft_printf_x(t_ull n, char cas);
char	*ft_printf_u(unsigned int n);
char	*ft_printf_d(int n);
int		ft_specs_search(const char *s);
int		ft_basecount(int n, int cnt, int base);
int		ft_u_basecount(t_ull n, int cnt, t_uint base);
void	ft_u_dectobase(char *str, t_ull n, t_uint pos, t_uint base);
#endif
