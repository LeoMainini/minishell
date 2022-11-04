/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:52:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/04 18:18:44 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

char	*remove_char(char *str, int index)
{
	int		size;
	char	*result;

	size = 0;
	while (str[size])
		size++;
	result = ft_calloc(size, sizeof(char));
	size = -1;
	while (str[++size])
	{
		if (size < index)
			result[size] = str[size];
		if (size > index)
			result[size - 1] = str[size];
	}
	free(str);
	return (result);
}

char *convert_to_value(char **str, int i, t_ms *data)
{
	int		j;
	char	**env_addr;
	char	*substr;
	char	**temp;

	j = 0;
	while ((*str)[(i + 1) + j] && !ft_isspace((*str)[(i + 1) + j]) && (*str)[(i + 1) + j] != '"')
		j++;
	substr = ft_substr(*str, i + 1, j);
	printf("var = %s\n len of name = %d\n", substr, j);
	env_addr = get_env(substr, data);
	free(substr);
	if (env_addr)
	{
		substr = *env_addr;
		temp = get_sep_env_values(substr);
		if (temp[1])
		{
			substr = ft_strdup(temp[1]);
			free(temp[1]);
			free(temp[0]);
			free(temp);
		}
		else
		{
			free(temp[0]);
			free(temp);
			substr = ft_strdup("");
		}
		printf("val = %s\n", substr);
	}
	*str = remove_char(*str, i);
	printf("%s    %c\n", *str, (*str)[i]);
	return (0);
}


void	interpret_strings(t_cmdd *argd, t_ms *data)
{
	int		i;
	int		k = -1;
	int		j;
	int		in_doubles = 0;
	int		in_singles = 0;
	char	*temp;
	char	*substr;

	(void)data;
	while (argd->args[++k])
	{
		i = -1;
		while (argd->args[k][++i])
		{
			if (argd->args[k][i] == '"' && !in_doubles && !in_singles)
			{
				argd->args[k] = remove_char(argd->args[k], i);
				in_doubles = 1;
			}
			else if (argd->args[k][i] == '"' && in_doubles && !in_singles)
			{
				argd->args[k] = remove_char(argd->args[k], i);
				in_doubles = 0;
			}
			if (argd->args[k][i] == 39 && !in_singles && !in_doubles)
			{
				argd->args[k] = remove_char(argd->args[k], i);
				in_singles = 1;
			}
			else if (argd->args[k][i] == 39 && in_singles && !in_doubles)
			{
				argd->args[k] = remove_char(argd->args[k], i);
				in_singles = 0;
			}
			if ((in_doubles || (!in_doubles && !in_singles)) && argd->args[k][i] == '$')
				convert_to_value(&argd->args[k], i, data);
		}
	}
}