/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 17:12:41 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/16 16:23:48 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>

int		ft_printf(const char *s, ...);
void	pf_printmemory(unsigned long n, int fd, char *base, int *counter);
void	pf_putbase10_plus(unsigned int n, int fd, char *base, int *counter);
void	pf_putchar_fd(char c, int fd, int *counter);
void	pf_putnbr_fd(int n, int fd, int *counter);
void	pf_putstr_fd(char *s, int fd, int *counter);
size_t	ft_strlen(const char *s);

#endif
