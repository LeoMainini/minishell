/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:52:29 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/19 16:59:20 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

int	convert_to_value(char **str, int i, t_ms *data)
{
	int		j;
	int		not_empty;

	j = 0;
	not_empty = 1;
	while ((*str)[(i + 1) + j] && !ispc((*str)[(i + 1) + j])
		&& (*str)[(i + 1) + j] != '"' && (*str)[(i + 1) + j] != 39
		&& (*str)[(i + 1) + j] != '$'
		&& (ft_isalnum((*str)[(i + 1) + j]) || (*str)[(i + 1) + j] == '?'
			|| (*str)[(i + 1) + j] == '_'))
		j++;
	if (!j && !ft_strncmp(&(*str)[i], "$$", 2))
	{
		*str = remove_char(*str, i);
		if (!*str)
			return (0);
		return (1);
	}
	not_empty = replace_value_str(str, i, data, j);
	return (not_empty);
}

int	handle_quotes(char **str, int i, int *in_doubles, int *in_singles)
{
	int	remove;

	remove = 0;
	if ((*str)[i] == '"' && !*in_doubles && !*in_singles)
	{
		remove = 1;
		*in_doubles = 1;
	}
	else if ((*str)[i] == '"' && *in_doubles && !*in_singles)
	{
		remove = 1;
		*in_doubles = 0;
	}
	if ((*str)[i] == 39 && !*in_singles && !*in_doubles)
	{
		remove = 1;
		*in_singles = 1;
	}
	else if ((*str)[i] == 39 && *in_singles && !*in_doubles)
	{
		remove = 1;
		*in_singles = 0;
	}
	return (remove);
}

int	check_str_char_condition(char **strs, int k, t_ms *data)
{
	int	i;
	int	inc;
	int	in_singles;
	int	in_doubles;

	i = 0;
	in_singles = 0;
	in_doubles = 0;
	while (strs[k][i])
	{
		inc = 0;
		if (!handle_quotes(&(strs[k]), i, &in_doubles, &in_singles))
			inc = 1;
		else
			strs[k] = remove_char(strs[k], i);
		if (((in_doubles || !in_doubles) && !in_singles)
			&& strs[k][i] == '$' && strs[k][i + 1])
				inc = convert_to_value(&strs[k], i, data);
		if (inc && strs[k][i])
			i++;
		else if (!strs[k][i])
			break ;
	}
	return (1);
}

void	interpret_strings(char **strs, t_ms *data)
{
	int		k;

	k = -1;
	while (strs[++k])
		check_str_char_condition(strs, k, data);
}
