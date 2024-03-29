/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:53:40 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/23 16:37:10 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

char	**alloc_envmem(char **envs, int offset)
{
	int		i;
	char	**temp;

	if (!envs)
		return (0);
	i = 0;
	while (envs[i])
		i++;
	temp = (char **)ft_calloc(i + offset + 1, sizeof(char *));
	if (!temp)
		return (0);
	return (temp);
}

char	**duplicate_envp(char **envs, int offset, int freeable)
{
	int		i;
	char	**temp;

	if (!envs)
		return (0);
	i = 0;
	while (envs[i])
		i++;
	temp = (char **)ft_calloc(i + offset + 1, sizeof(char *));
	if (!temp)
		return (0);
	while (i--)
	{
		temp[i] = ft_strdup(envs[i]);
		if (!temp[i])
		{
			free_strarray(temp);
			if (freeable)
				free_strarray(g_envs);
			return (0);
		}
		if (freeable)
			check_free_zeroout((void **)&envs[i]);
	}
	return (temp);
}
