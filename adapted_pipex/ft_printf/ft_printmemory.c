/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 19:32:55 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/16 16:24:02 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

static void	clean_temp3(char *temp)
{
	int	i;

	i = -1;
	while (++i < 10)
		temp[i] = '\0';
}

static int	output_index_generator3(unsigned long n_l, char *temp, char *base)
{
	int	i;
	int	basesize;

	basesize = ft_strlen(base);
	clean_temp3(temp);
	temp[17] = '\0';
	i = 16;
	while (n_l != 0 && i >= 0)
	{
		temp[i] = base[n_l % basesize];
		i--;
		n_l = n_l / basesize;
	}
	i++;
	return (i);
}

void	pf_printmemory(unsigned long n, int fd, char *base, int *counter)
{
	int		i;
	char	temp[18];

	if (n == 0)
		pf_putstr_fd("0x0", fd, counter);
	else
	{
		i = 0;
		i = output_index_generator3(n, temp, base);
		pf_putstr_fd("0x", 1, counter);
		pf_putstr_fd(&temp[i], fd, counter);
	}
}
