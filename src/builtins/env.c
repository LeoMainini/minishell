/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:29:45 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 14:51:03 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

extern char	**g_envs;

int	env(t_cmdd *argd, t_ms *data)
{
	int	i;

	if (argd->args[1] && ft_putstr_fd("Too many arguments.\n", STDERR_FILENO))
		return (set_ret_return(data, 127));
	i = -1;
	while (g_envs[++i])
		if (ft_strchr(g_envs[i], '='))
			ft_putendl_fd(g_envs[i], argd->out_fd);
	return (set_ret_return(data, 0));
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
	while (split_envs[++i])
	{
		k = -1;
		while (split_envs[++k])
			if (scmp(split_envs[i][0], split_envs[k][0]) < 0)
				ft_str_swap(&split_envs[i], &split_envs[k]);
	}
	print_free_3darray(&split_envs, fd);
	return (0);
}

int	is_alphastr(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if ((str[i] < 'a' || str[i] > 'z') && (str[i] < 'A' || str[i] > 'Z'))
			return (0);
	return (1);
}
