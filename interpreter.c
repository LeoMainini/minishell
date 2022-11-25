/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:52:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/25 04:00:56 by ben              ###   ########.fr       */
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

char	*replace_name(char *str, char *env, int i, int j)
{
	char	*result;
	char	*temp;

	temp = ft_substr(str, 0, i);
	result = ft_strjoin("", temp);
	free(temp);
	result = ft_strfree_join(&result, env);
	temp = ft_substr(str, i + j, ft_strlen(str) - i - j);
	result = ft_strfree_join(&result, temp);
	free(temp);
	free(str);
	return (result);
}

char	*get_env_value(char *env_name, t_ms *data)
{
	char	**env_addr;
	char	**temp;

	env_addr = 0;
	env_addr = get_env(env_name, data);
	free(env_name);
	if (env_addr)
	{
		env_name = *env_addr;
		temp = get_sep_env_values(env_name);
		if (temp[1])
			env_name = ft_strdup(temp[1]);
		else
			env_name = ft_strdup("");
		free(temp[1]);
		free(temp[0]);
		free(temp);
	}
	else
		env_name = ft_strdup("");
	return (env_name);
}

int	convert_to_value(char **str, int i, t_ms *data)
{
	int		j;
	char	*substr;
	int		not_empty;

	j = 0;
	not_empty = 1;
	while ((*str)[(i + 1) + j] && !ft_isspace((*str)[(i + 1) + j])
		&& (*str)[(i + 1) + j] != '"' && (*str)[(i + 1) + j] != 39 
		&& (*str)[(i + 1) + j] != '$')
		j++;
	substr = ft_substr(*str, i + 1, j);
	if (!ft_strcmp(substr, "?"))
	{
		free(substr);
		substr = ft_itoa(data->ret);
		*str = remove_char(*str, i);
		*str = replace_name(*str, substr, i, j);
	}
	else
	{
		substr = get_env_value(substr, data);
		*str = remove_char(*str, i);
		*str = replace_name(*str, substr, i, j);
		if (!ft_strcmp(substr, ""))
			not_empty = 0;
	}
	free(substr);
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

int	interpret_strings(char **strs, t_ms *data)
{
	int		i;
	int		k;
	int		in_doubles;
	int		in_singles;
	int		inc;

	k = -1;
	in_doubles = 0;
	in_singles = 0;
	while (strs[++k])
	{
		i = 0;
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
				break;
		}
	}
	return (!in_doubles && !in_singles);
}