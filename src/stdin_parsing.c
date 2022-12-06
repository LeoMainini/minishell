/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 17:15:06 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 15:35:18 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

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
	int		result;

	result = 0;
	temp = 0;
	temp = ft_strjoin(limiter, "\n");
	if (str_array && str_array[index])
		result = scmp(str_array[index], temp);
	if (!result)
	{
		if (!null_index_delta)
			free(str_array[index]);
		str_array[index + null_index_delta] = 0;
	}
	free(temp);
	return (result == 0);
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

char	*get_nlended_line(char *prompt)
{
	char	*str;

	str = readline(prompt);
	if (str)
		str = ft_strfree_join(&str, "\n");
	return (str);
}

char	**parse_stdin_tolimit(char *limiter)
{
	char	**lines_in;
	int		k;

	lines_in = (char **) malloc(2 * sizeof(char *));
	if (!lines_in)
		return (0);
	k = 0;
	lines_in[k] = get_nlended_line("> ");
	if (check_limiter(lines_in, k, limiter, 1))
	{
		free(lines_in[k]);
		lines_in[k] = ft_strdup("");
		return (lines_in);
	}
	while (lines_in[k++])
	{
		lines_in[k] = get_nlended_line("> ");
		extend_array(&lines_in, k);
		if (check_limiter(lines_in, k, limiter, 0))
			break ;
	}
	return (lines_in);
}
