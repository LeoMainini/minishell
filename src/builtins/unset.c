/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:21:44 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/17 17:48:32 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

void	remove_env_index(int found_i, char ***temp)
{
	int	k;

	k = -1;
	while (g_envs[++k])
	{
		if (k < found_i)
			(*temp)[k] = ft_strdup(g_envs[k]);
		if (k > found_i)
			(*temp)[k - 1] = ft_strdup(g_envs[k]);
		check_free_zeroout((void **)&g_envs[k]);
	}
}

int	unset(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int		i;
	int		found_i;
	int		result;
	char	**temp;

	i = 0;
	result = 0;
	while (!before_pipe && argd->args[++i])
	{
		found_i = get_index(argd->args[i]);
		if ((name_invalid(argd->args[i], 1) || found_i == -1))
		{
			if (name_invalid(argd->args[i], 0))
				result = 1;
			continue ;
		}
		temp = alloc_envmem(g_envs, -1);
		if (!temp)
			return (0);
		remove_env_index(found_i, &temp);
		check_free_zeroout((void **)&g_envs);
		g_envs = temp;
	}
	return (set_ret_return(data, result));
}
