/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:29:45 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/23 15:20:03 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

extern char **g_envs;

int	env(t_cmdd *argd, t_ms *data)
{
	int	i;
	
	if (argd->args[1] && printf("Too many arguments.\n"))
		return (set_ret_return(data, 1));
	i = -1;
	while (g_envs[++i])
		if (ft_strchr(g_envs[i], '='))
			ft_putendl_fd(g_envs[i], argd->out_fd);
	return (set_ret_return(data, 1));
}

char	**get_sep_env_values(char *str)
{
	char	**index;

	if (ft_strchr(str, '='))
	{
		index = ft_calloc(3, sizeof(char *));
		index[0] = ft_substr(str, 0, (ft_strchr(str, '=') - str));
		index[1] = ft_substr(str, (ft_strchr(str, '=') - str) + 1,
			ft_strlen(str) - (ft_strchr(str, '=') - str));
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

int	get_index(char	*env)
{
	int		i;
	int		k;
	int		result;
	char	**sep_env;

	i = -1;
	result = -1;
	while (g_envs[++i])
	{
		k = -1;
		sep_env = get_sep_env_values(g_envs[i]);
		if (!ft_strcmp((const char*)env, (const char*)(sep_env[0])))
		{
			while (sep_env[++k])
				free(sep_env[k]);
			free(sep_env);
			result = i;
			break ;
		}
		while (sep_env[++k])
			free(sep_env[k]);
		free(sep_env);
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

void	print_free_3darray(char ****str_array, int fd)
{
	char	***strs;
	int		i;

	strs = *str_array;
	i = -1;
	while (strs[++i])
	{
		if (strs[i][1])
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(strs[i][0], fd);
			ft_putchar_fd('"', fd);
			ft_putstr_fd(strs[i][1], fd);
			ft_putstr_fd("\"\n", fd);
			free(strs[i][0]);
			free(strs[i][1]);
		}
		else
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(strs[i][0], fd);
			free(strs[i][0]);
		}
		free(strs[i]);
	}
	free(strs);
}

int	print_sorted_envs(int fd)
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
	print_free_3darray(&split_envs, fd);
	return (0);
}

int	export(t_cmdd *argd, t_ms *data, int before_pipe)
{
	char	**envi;
	int		last_i;
	char	*name;
	int		i;

	envi = 0;
	if (!argd->args[1])
		return (print_sorted_envs(argd->out_fd));
	i = 0;
	while (!before_pipe && argd->args[++i])
	{
		if (ft_strchr(argd->args[i], '='))
			name = ft_substr(argd->args[i], 0,
				(ft_strchr(argd->args[i], '=') - argd->args[i]));
		else 
			name = ft_strdup(argd->args[i]);
		envi = get_env(name, data);
		if (envi)
		{
			if (ft_strchr(argd->args[i], '='))
			{
				free(*envi);
				*envi = ft_strdup(argd->args[i]);
			}
			free(name);
			break ;
		}
		free(name);
		envi = g_envs;
		g_envs = duplicate_envp(g_envs, 1);
		free(envi);
		last_i = 0;
		while (g_envs[last_i])
			last_i++;
		g_envs[last_i] = ft_strdup(argd->args[i]);
	}
	return (set_ret_return(data, 0));
}

int	unset(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int		i;
	int		k;
	int		found_i;
	char	**temp;

	i = 0;
	while (!before_pipe && argd->args[++i])
	{
		found_i = get_index(argd->args[i]);
		if (found_i == -1)
			continue ;
		temp = alloc_envmem(g_envs, -1);
		k = -1;
		while (g_envs[++k])
		{
			if (k < found_i)
				temp[k] = ft_strdup(g_envs[k]);
			if (k > found_i)
				temp[k - 1] = ft_strdup(g_envs[k]);
			free(g_envs[k]);
		}
		free(g_envs);
		g_envs = temp;
	}
	return (set_ret_return(data, 0));
}