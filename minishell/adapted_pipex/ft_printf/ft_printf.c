/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 14:18:03 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/16 16:23:24 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h" 
#include <stdio.h>

static	void	check_type(char c, va_list value, int	*n)
{
	if (c == 'c')
		pf_putchar_fd((char)va_arg(value, int), 1, n);
	else if (c == 's')
		pf_putstr_fd((char *)va_arg(value, int *), 1, n);
	else if (c == 'p')
		pf_printmemory(va_arg(value, unsigned long), 1,
			"0123456789abcdef", n);
	else if (c == 'd' || c == 'i')
		pf_putnbr_fd((int)va_arg(value, int), 1, n);
	else if (c == 'u')
		pf_putbase10_plus(va_arg(value, unsigned int), 1, "0123456789", n);
	else if (c == 'x')
		pf_putbase10_plus(va_arg(value, unsigned int), 1,
			"0123456789abcdef", n);
	else if (c == 'X')
		pf_putbase10_plus(va_arg(value, unsigned int), 1,
			"0123456789ABCDEF", n);
	else if (c == '%')
		pf_putchar_fd('%', 1, n);
}

int	ft_printf(const char *s, ...)
{
	va_list	value;
	int		counter;
	int		i;

	counter = 0;
	i = -1;
	va_start(value, s);
	while (s[++i])
	{
		if (s[i] == '%')
		{
			check_type(s[i + 1], value, &counter);
			i++;
		}
		else
			pf_putchar_fd(s[i], 1, &counter);
	}
	va_end(value);
	return (counter);
}
