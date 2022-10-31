/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:52:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/31 17:49:34 by leferrei         ###   ########.fr       */
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
			if (in_doubles && argd->args[k][i] == '$')
			{
				j = 0;
				while (!ft_isspace(argd->args[k][(i + 1) + j]) && argd->args[k][(i + 1) + j] != '"')
					j++;
				substr = ft_substr(argd->args[k], i + 1, j);
				printf("var = %s\n", substr);
				temp = *get_env(substr, data);
				if (temp)
					temp = &temp[ft_strlen(substr) + 1];
				printf("val = %s\n", temp);
			}

		}
	}
}