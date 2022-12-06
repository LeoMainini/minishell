/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:21:44 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/06 16:20:16 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

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
