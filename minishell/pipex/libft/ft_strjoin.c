/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:17:02 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/23 17:30:53 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../ft_printf/ft_printf.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	int		k;
	char	*result;

	i = 0;
	while (s1 && (s1)[i] != '\0')
		i++;
	j = 0;
	while ((s2)[j] != '\0')
		j++;
	result = (char *)malloc((i + j + 1) * sizeof(*result));
	if (result == NULL)
		return (NULL);
	result[i + j] = '\0';
	k = -1;
	while (s1 && (s1)[++k] != '\0')
		result[k] = (s1)[k];
	k = -1;
	while ((s2)[++k] != '\0')
		result[i + k] = (s2)[k];
	return (&result[0]);
}
