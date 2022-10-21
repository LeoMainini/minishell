/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 13:17:38 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/23 17:50:04 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int		i;

	if (!s)
		return ;
	i = -1;
	while (s[++i] != '\0')
		f((unsigned int)i, &s[i]);
}
