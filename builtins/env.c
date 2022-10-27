/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:29:45 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/27 17:22:07 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

extern char **g_envs;

int	env(t_cmdd *argd, t_ms *data)
{
	int	i;
	
	//TODO:Should not show variables without valye
	if (argd->args[1] && printf("Too many arguments.\n"))
		return (set_ret_return(data, 1));
	i = -1;
	while (g_envs[++i])
		if (ft_strchr(g_envs[i], '='))
			printf("%s\n", g_envs[i]);
	return (set_ret_return(data, 1));
}

char	**get_env(char *env, t_ms *data)
{
	int	i;
	int	size;

	i = -1;
	while (g_envs[++i])
		if (!ft_strncmp((const char*)env, (const char*)(g_envs[i]), ft_strlen(env)))
			break ;
	size = 0;
	while (g_envs[size])
		size++;
	if (i == size)
	{
		set_ret_return(data, 127);
		return (0);
	}
	return (&g_envs[i]);
}

void	ft_str_swap(char ***s1, char ***s2)
{
	char	**temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

char	**get_sep_env_values(char *str)
{
	char	**index;

	if (ft_strchr(str, '='))
	{
		index = ft_calloc(3, sizeof(char *));
		index[0] = ft_substr(str, 0, (ft_strchr(str, '=') - str));
		index[1] = ft_substr(str, (ft_strchr(str, '=') - str) + 1, ft_strlen(str) - (ft_strchr(str, '=') - str));
		index[2] = 0;
	}
	else 
	{
		index = ft_calloc(2, sizeof(char *));
		index[0] = ft_strdup(str);
		index[1] = 0;
	}
	return (index);
}

void	print_free_3darray(char ****str_array)
{
	char	***strs;
	int		i;

	strs = *str_array;
	i = -1;
	while (strs[++i])
	{
		if (strs[i][1])
		{
			printf("declare -x %s=\"%s\"\n", strs[i][0], strs[i][1]);
			free(strs[i][0]);
			free(strs[i][1]);
		}
		else
		{
			printf("declare -x %s\n", strs[i][0]);
			free(strs[i][0]);
		}
		free(strs[i]);
	}
	free(strs);
}

void	print_sorted_envs()
{
	char	***split_envs;
	int		i;
	int		k;

	i = 0;
	while (g_envs[i])
		i++;
	split_envs = (char ***)ft_calloc(i + 1, sizeof(char *));
	i = -1;
	while (g_envs[++i])
		split_envs[i] = get_sep_env_values(g_envs[i]);
	i = -1;
	while(split_envs[++i])
	{
		k = -1;
		while (split_envs[++k])
			if (ft_strcmp(split_envs[i][0], split_envs[k][0]) < 0)
				ft_str_swap(&split_envs[i], &split_envs[k]);
	}
	print_free_3darray(&split_envs);
}

int	export(t_cmdd *argd, t_ms *data)
{
	char	**envi;
	int		last_i;
	char	*name;

	envi = 0;
	if (!argd->args[1])
		print_sorted_envs();
	if (argd->args[2] && printf("Too many arguments\n"))
		return (set_ret_return(data, 1));
	//TODO:Remove too many argyments flag and make export execute on every argv
	if (ft_strchr(argd->args[1], '='))
		name = ft_substr(argd->args[1], 0, (ft_strchr(argd->args[1], '=') - argd->args[1]));
	else 
		name = ft_strdup(argd->args[1]);
	envi = get_env(name, data);
	if (envi)
	{
		if (ft_strchr(argd->args[1], '='))
		{
			free(*envi);
			*envi = ft_strdup(argd->args[1]);
		}
		free(name);
		return (set_ret_return(data, 0));
	}
	free(name);
	envi = g_envs;
	g_envs = duplicate_envp(g_envs, 1);
	free(envi);
	last_i = 0;
	while (g_envs[last_i])
		last_i++;
	g_envs[last_i] = ft_strdup(argd->args[1]);
	return (set_ret_return(data, 0));
}