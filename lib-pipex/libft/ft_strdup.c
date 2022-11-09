/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 19:02:21 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/21 12:41:42 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	int		size;
	char	*r;

	size = 0;
	while (s[size] != '\0')
		size++;
	r = (char *)malloc(size + 1 * (sizeof(*r)));
	if (r == NULL)
		return (NULL);
	r[size] = '\0';
	size = -1;
	while (s[++size] != '\0')
		r[size] = s[size];
	return (r);
}
