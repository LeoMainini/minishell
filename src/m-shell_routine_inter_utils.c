/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell_routine_inter_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 14:56:13 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/28 14:59:55 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split_inter_data(t_spl *spl, char **split_out, int i, int toggle)
{
	if (toggle)
	{
		check_free_zeroout((void **)&spl->ss[i][0]);
		check_free_zeroout((void **)&spl->ss[i]);
	}
	check_free_zeroout((void **)&split_out);
}

char	**alloc_result_mem( t_spl *spl, int i, char **split_out)
{
	int	k;
	int	j;

	if (!split_out)
		return (0);
	k = 0;
	while (split_out[k])
		k++;
	j = 0;
	while (spl->ss[i][j])
		j++;
	return (ft_calloc(j + k, sizeof(char *)));
}
