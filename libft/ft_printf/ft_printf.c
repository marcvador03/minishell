/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfleury <mfleury@student.42barcelona.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:24:11 by mfleury           #+#    #+#             */
/*   Updated: 2024/08/06 16:23:23 by mfleury          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_args(va_list args, char *flag_str, int *cnt)
{
	char	*str;

	str = NULL;
	if (*flag_str == 's')
		str = ft_printf_s(va_arg(args, char *));
	if (*flag_str == 'x' || *flag_str == 'X')
		str = ft_printf_x(va_arg(args, unsigned int), *flag_str);
	if (*flag_str == 'p')
		str = ft_printf_x(va_arg(args, unsigned long long), *flag_str);
	if (*flag_str == 'd' || *flag_str == 'i')
		str = ft_printf_d(va_arg(args, int));
	if (*flag_str == 'u')
		str = ft_printf_u((int)va_arg(args, unsigned int));
	if (*flag_str == '%')
		str = ft_strdup("%");
	*cnt += ft_strlen(str);
	if (*flag_str == 'c')
	{
		ft_printf_c(va_arg(args, int), cnt);
		return (str);
	}
	if (str == NULL)
		*cnt = -1;
	return (str);
}

int	ft_printf_flag_init(const char *s, char **str, va_list args, int *cnt)
{
	char		*flag_str;
	int			flag_len;

	flag_len = ft_specs_search(s) + 1;
	flag_str = ft_substr(s, 0, flag_len);
	if (flag_str == NULL)
		return (0);
	*str = ft_args(args, flag_str + 1, cnt);
	if (str == NULL)
		return (free(flag_str), 0);
	free(flag_str);
	return (flag_len);
}

int	ft_printf(const char *s, ...)
{
	va_list		args;
	char		*str;
	int			cnt;

	va_start(args, s);
	cnt = 0;
	while (*s != '\0')
	{
		if (*s == '%' && *(s + 1) != '\0')
		{
			s = s + ft_printf_flag_init(s, &str, args, &cnt);
			if (cnt == -1)
				return (free(str), va_end(args), -1);
			ft_putstr_fd(str, 1);
			free(str);
			if (*s == '\0')
				return (va_end(args), cnt);
		}
		else if (*s == '%' && *(s + 1) == '\0')
			return (va_end(args), -1);
		else if (cnt++ >= 0)
			ft_putchar_fd(*s++, 1);
	}
	return (va_end(args), cnt);
}
