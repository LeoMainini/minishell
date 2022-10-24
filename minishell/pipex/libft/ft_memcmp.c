/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 18:47:30 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/18 17:07:35 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	**b_s1;
	unsigned char	**b_s2;

	b_s1 = (unsigned char **)&s1;
	b_s2 = (unsigned char **)&s2;
	if (n == 0)
		return (0);
	while (**b_s1 == **b_s2 && n > 1)
	{
		(*b_s1)++;
		(*b_s2)++;
		n--;
	}
	return (**b_s1 - **b_s2);
}
