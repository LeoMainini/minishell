/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:12:06 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/18 16:50:16 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	char	**r;

	r = (char **)&big;
	if (little[0] == '\0')
		return (&(*r)[0]);
	i = -1;
	while (big[++i] != '\0' && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && little[j] != '\0'
			&& i + j < len && big[i + j] != '\0')
		{
			if (little[j + 1] == '\0')
				return (&(*r)[i]);
			j++;
		}	
	}
	return (NULL);
}
