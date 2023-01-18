/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:29:49 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/18 15:39:24 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

char	**get_sep_env_values(char *str)
{
	char	**index;

	if (ft_strchr(str, '='))
	{
		index = ft_calloc(3, sizeof(char *));
		if (!index)
			return (0);
		index[0] = ft_substr(str, 0, (ft_strchr(str, '=') - str));
		index[1] = ft_substr(str, (ft_strchr(str, '=') - str) + 1,
				ft_strlen(str) - (ft_strchr(str, '=') - str));
		index[2] = 0;
	}
	else
	{
		index = ft_calloc(2, sizeof(char *));
		if (!index)
			return (0);
		index[0] = ft_strdup(str);
		index[1] = 0;
	}
	return (index);
}

void	free_sep_env_mem(char ***sep_env)
{
	int	k;

	k = -1;
	while ((*sep_env)[++k])
		check_free_zeroout((void **)&(*sep_env)[k]);
	check_free_zeroout((void **)sep_env);
}

int	get_index(char	*env)
{
	int		i;
	int		result;
	char	**sep_env;

	i = -1;
	result = -1;
	while (g_envs[++i])
	{
		sep_env = get_sep_env_values(g_envs[i]);
		if (!sep_env)
			return (-1);
		if (!scmp((const char *)env, (const char *)(sep_env[0])))
		{
			free_sep_env_mem(&sep_env);
			result = i;
			break ;
		}
		free_sep_env_mem(&sep_env);
	}
	return (result);
}

char	**get_env(char *env, t_ms *data)
{
	int	i;

	(void)data;
	i = -1;
	i = get_index(env);
	if (i == -1)
		return (0);
	return (&g_envs[i]);
}

void	ft_str_swap(char ***s1, char ***s2)
{
	char	**temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}
