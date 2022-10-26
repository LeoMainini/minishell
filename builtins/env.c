/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:29:45 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/26 17:39:38 by leferrei         ###   ########.fr       */
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

int	export(t_cmdd *argd, t_ms *data)
{
	char	**env_d;
	char	**envi;
	int		last_i;

	envi = 0;
	if (!argd->args[1])
		return (env(argd, data));
	if (argd->args[2] && printf("Too many arguments\n"))
		return (set_ret_return(data, 1));
	env_d = ft_split(argd->args[1], '=');
	envi = get_env(env_d[0], data);
	if (envi)
	{
		printf("got here\n");
		free(*envi);
		*envi = ft_strdup(argd->args[1]);
		return (set_ret_return(data, 0));
	}
	envi = g_envs;
	g_envs = duplicate_envp(g_envs, 1);
	free(envi);
	last_i = 0;
	while (g_envs[last_i])
		last_i++;
	g_envs[last_i] = ft_strdup(argd->args[1]);
	return (set_ret_return(data, 0));
}
