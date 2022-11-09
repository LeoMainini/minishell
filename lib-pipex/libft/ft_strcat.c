/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 15:07:07 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/18 16:52:08 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strcat(char *dest, const char *src)
{
	int	index;
	int	jindex;

	index = 0;
	while (dest[index] != '\0')
		index++;
	jindex = -1;
	while (src[++jindex] != '\0')
		dest[index + jindex] = src[jindex];
	dest[jindex + index] = '\0';
	return (dest);
}
