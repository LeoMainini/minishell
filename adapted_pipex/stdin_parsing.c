/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 17:15:06 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/12 17:15:07 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line/get_next_line.h"

char	*ft_strfreedup(char *s)
{
	int		i;
	char	*res;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != 0)
		i++;
	res = (char *)malloc((i + 1) * sizeof(char));
	if (!res)
		return (0);
	i = -1;
	while (s[++i] != 0)
		res[i] = s[i];
	res[i] = 0;
	free(s);
	return (res);
}

static int	check_limiter(char **str_array, int index, char *limiter,
							int null_index_delta)
{
	char	*temp;
	int		limitr_diff;

	temp = 0;
	temp = ft_strjoin(limiter, "\n");
	limitr_diff = ft_strcmp(str_array[index], temp);
	if (!limitr_diff)
	{
		if (!null_index_delta)
			free(str_array[index]);
		str_array[index + null_index_delta] = 0;
	}
	free(temp);
	return (limitr_diff == 0);
}

static int	extend_array(char ***str_array, int k)
{
	char	**temp;
	int		i;

	temp = (char **) ft_calloc((k + 2), sizeof(char *));
	if (!temp)
		return (0);
	i = -1;
	while (++i <= k)
		temp[i] = ft_strfreedup((*str_array)[i]);
	temp[i] = 0;
	free(*str_array);
	*str_array = temp;
	return (1);
}

char	**parse_stdin_tolimit(char *limiter)
{
	char	**lines_in;
	int		k;

	lines_in = (char **) malloc(2 * sizeof(char *));
	if (!lines_in)
		return (0);
	k = 0;
	lines_in[k] = get_next_line(STDIN_FILENO);
	if (check_limiter(lines_in, k, limiter, 1))
		return (lines_in);
	while (lines_in[k++])
	{
		lines_in[k] = ft_strfreedup(get_next_line(STDIN_FILENO));
		extend_array(&lines_in, k);
		if (check_limiter(lines_in, k, limiter, 0))
			break ;
	}
	return (lines_in);
}
