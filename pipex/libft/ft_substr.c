/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:11:45 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/23 17:31:18 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*result;
	unsigned int	size;
	size_t			i;

	if (!s)
		return (NULL);
	size = (unsigned int)ft_strlen(s);
	if (start >= size)
		return (ft_strdup(""));
	i = 0;
	while (s[start + i] != '\0' && i < len)
		i++;
	result = (char *)malloc((i + 1) * sizeof(*result));
	if (result == NULL)
		return (NULL);
	i = -1;
	while (s[start + ++i] != '\0' && i < len)
		result[i] = s[start + i];
	result[i] = '\0';
	return (result);
}
/*
int main(void)
{
	char *test = ft_substr("Ola o meu nome e leo", 0, 20);
	ft_putendl_fd(test, 1);
}*/
