/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:12:06 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/17 13:23:20 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

char	*ft_strstr(const char *haystack, const char *needle)
{
	int		i;
	int		j;
	char	**r;

	r = (char **)&haystack;
	if (needle[0] == '\0')
		return (&(*r)[0]);
	i = -1;
	while (haystack[++i] != '\0')
	{
		j = 0;
		while (haystack[i + j] == needle[j] && needle[j] != '\0')
		{
			if (needle[j + 1] == '\0')
				return (&(*r)[i]);
			j++;
		}	
	}
	return (NULL);
}
