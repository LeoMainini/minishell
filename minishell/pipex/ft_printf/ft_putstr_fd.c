/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 19:10:00 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/15 17:20:26 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	pf_putstr_fd(char *s, int fd, int *counter)
{
	if (s != NULL)
		while (*s)
			pf_putchar_fd(*(s++), fd, counter);
	else
		pf_putstr_fd("(null)", 1, counter);
}
