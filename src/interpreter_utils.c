/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:09:35 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/28 13:52:23 by leferrei         ###   ########.fr       */
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
	if (!result)
		return (0);
	size = -1;
	while (str[++size])
	{
		if (size < index)
			result[size] = str[size];
		if (size > index)
			result[size - 1] = str[size];
	}
	check_free_zeroout((void **)&str);
	return (result);
}

char	*replace_name(char *str, char *env, int i, int j)
{
	char	*result;
	char	*temp;

	temp = ft_substr(str, 0, i);
	result = ft_strjoin("", temp);
	check_free_zeroout((void **)&temp);
	result = ft_strfree_join(&result, env);
	temp = ft_substr(str, i + j, ft_strlen(str) - i - j);
	result = ft_strfree_join(&result, temp);
	check_free_zeroout((void **)&temp);
	check_free_zeroout((void **)&str);
	return (result);
}

char	*get_env_value(char *env_name, t_ms *data)
{
	char	**env_addr;
	char	**temp;

	env_addr = 0;
	env_addr = get_env(env_name, data);
	check_free_zeroout((void **)&env_name);
	if (env_addr)
	{
		env_name = *env_addr;
		temp = get_sep_env_values(env_name);
		if (temp[1])
			env_name = ft_strdup(temp[1]);
		else
			env_name = ft_strdup("");
		check_free_zeroout((void **)&temp[1]);
		check_free_zeroout((void **)&temp[0]);
		check_free_zeroout((void **)&temp);
	}
	else
		env_name = ft_strdup("");
	return (env_name);
}

int	replace_value_str(char **str, int i, t_ms *data, int j)
{
	char	*substr;
	int		not_empty;

	not_empty = 1;
	substr = ft_substr(*str, i + 1, j);
	if (!scmp(substr, "?"))
	{
		check_free_zeroout((void **)&substr);
		substr = ft_itoa(data->ret);
		*str = remove_char(*str, i);
		*str = replace_name(*str, substr, i, j);
	}
	else
	{
		substr = get_env_value(substr, data);
		*str = remove_char(*str, i);
		*str = replace_name(*str, substr, i, j);
		if (!scmp(substr, ""))
			not_empty = 0;
	}
	check_free_zeroout((void **)&substr);
	return (not_empty);
}
