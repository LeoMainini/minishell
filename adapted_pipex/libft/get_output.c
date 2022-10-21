/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_output.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:34:32 by leferrei          #+#    #+#             */
/*   Updated: 2022/07/27 15:35:18 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

char	*ft_strfree_join(char **s1, char *s2)
{
	char	*result;

	result = ft_strjoin(*s1, s2);
	free(*s1);
	return (result);
}

char	*get_output(int fd)
{
	int		read_bytes;
	char	buf[4096];
	char	*output;

	output = 0;
	ft_bzero(buf, 4096);
	read_bytes = read(fd, buf, sizeof(buf) - 1);
	if (read_bytes > 0)
		output = ft_strfree_join(&output, buf);
	while (read_bytes)
	{
		ft_bzero(buf, sizeof(buf));
		read_bytes = read(fd, buf, sizeof(buf) - 1);
		if (read_bytes < 0)
			exit(1);
		if (read_bytes && *buf)
			output = ft_strfree_join(&output, buf);
		if (!read_bytes)
			break ;
	}
	return (output);
}
