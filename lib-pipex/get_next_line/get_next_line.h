/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:41:59 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/10 17:02:34 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

char	*get_next_line(int fd);
char	*ft_sj(char *read_data, char *buf);
void	*gnl_calloc(size_t count, size_t size);
size_t	gnl_strlen(const char *s);
int		clean_buf(char *buf);

#endif
