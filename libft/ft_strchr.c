/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 12:44:34 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/18 16:55:36 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	**sp;

	sp = (char **)&s;
	i = -1;
	while ((*sp)[++i] != '\0')
		if ((*sp)[i] == (unsigned char)c)
			return (&(*sp)[i]);
	if (s[i] == (unsigned char)c)
		return (&(*sp)[i]);
	return (NULL);
}
