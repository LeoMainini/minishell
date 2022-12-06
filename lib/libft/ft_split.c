/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:44:32 by leferrei          #+#    #+#             */
/*   Updated: 2022/03/15 18:09:24 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	char_counter(char const *s, char c)
{
	size_t	count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**dst;
	char	*str;
	int		i;

	dst = (char **)malloc(sizeof(char *) * (char_counter(s, c) + 1));
	if (!s || !dst)
		return (0);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			str = (char *)s;
			while (*s && *s != c)
				s++;
			dst[i] = (char *)malloc(s - str + 1);
			ft_strlcpy(dst[i++], str, s - str + 1);
		}
		else
			s++;
	}
	dst[i] = NULL;
	return (dst);
}
