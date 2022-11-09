/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 19:32:55 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/15 17:14:47 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

static void	clean_temp2(char *temp)
{
	int	i;

	i = -1;
	while (++i < 10)
		temp[i] = '\0';
}

static int	output_index_generator2(unsigned int n_l, char *temp, char *base)
{
	int	i;
	int	basesize;

	basesize = ft_strlen(base);
	clean_temp2(temp);
	temp[11] = '\0';
	i = 10;
	while (n_l != 0 && i >= 0)
	{
		temp[i] = base[n_l % basesize];
		i--;
		n_l = n_l / basesize;
	}
	i++;
	return (i);
}

void	pf_putbase10_plus(unsigned int n, int fd, char *base, int *counter)
{
	int		i;
	char	temp[12];

	if (n == 0)
		pf_putchar_fd('0', fd, counter);
	else
	{
		i = 0;
		i = output_index_generator2(n, temp, base);
		pf_putstr_fd(&temp[i], fd, counter);
	}
}
