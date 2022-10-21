/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 19:32:55 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/22 21:06:00 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

static void	clean_temp(char *temp)
{
	int	i;

	i = -1;
	while (++i < 10)
		temp[i] = '\0';
}

int	output_index_generator(long n_l, int neg, char *temp)
{
	int	i;

	clean_temp(temp);
	temp[11] = '\0';
	i = 10;
	while (n_l != 0 && i >= 0)
	{
		temp[i] = "0123456789"[n_l % 10];
		i--;
		n_l = n_l / 10;
	}
	if (neg == 1)
		temp[i] = '-';
	else
		i++;
	return (i);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	n_l;
	int		i;
	char	temp[12];
	int		neg;

	n_l = (long)n;
	neg = 0;
	if (n_l < 0)
	{
		neg = 1;
		n_l = -n_l;
	}
	if (n_l == 0)
		ft_putchar_fd('0', fd);
	else
	{
		i = 0;
		i = output_index_generator(n_l, neg, temp);
		ft_putstr_fd(&temp[i], fd);
	}
}
