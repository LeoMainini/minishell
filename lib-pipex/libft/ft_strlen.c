/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/06 18:58:59 by leferrei          #+#    #+#             */
/*   Updated: 2022/02/17 13:40:52 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "../ft_printf/ft_printf.h"

size_t	ft_strlen(const char *s)
{
	int	size;

	if (!s)
		return (0);
	ft_printf("string pointer = %p\n", s);
	size = 0;
	while (s[size] != '\0')
		size++;
	return (size);
}
