/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 12:31:58 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/23 17:29:32 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*result;

	if (!s)
		return (NULL);
	result = ft_strdup(s);
	if (result == NULL)
		return (NULL);
	i = -1;
	while (s[++i] != '\0')
		result[i] = f((unsigned int)i, s[i]);
	return (result);
}
