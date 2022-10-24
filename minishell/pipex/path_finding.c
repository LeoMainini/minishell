/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finding.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 16:13:52 by leferrei          #+#    #+#             */
/*   Updated: 2022/09/29 16:13:53 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	steps_back(t_vars *data, int i)
{
	char	**file_path;
	int		k;
	int		steps_back;

	k = -1;
	steps_back = 0;
	file_path = ft_split(data->cmds[i][0], '/');
	while (file_path[++k])
		if (!ft_strcmp(file_path[k], ".."))
			steps_back++;
	k = -1;
	while (file_path[++k])
		free(file_path[k]);
	free(file_path);
	return (steps_back);
}

char	*join_chunks(char **str_chunks, char *sep, int limiter)
{
	int		k;
	char	*complete_str;

	k = -1;
	complete_str = 0;
	while (++k < limiter)
	{
		if (!complete_str)
			complete_str = ft_strjoin(sep, str_chunks[k]);
		else
		{
			complete_str = ft_strfree_join(&complete_str, sep);
			complete_str = ft_strfree_join(&complete_str, str_chunks[k]);
		}
	}
	if (complete_str)
		return (ft_strfree_join(&complete_str, sep));
	return (complete_str);
}

char	*absolute_to_relative_pwd(t_vars *data, int i, char *pwd)
{
	int		sb;
	char	**pwd_chunks;
	char	*relative_pwd;
	int		k;

	sb = steps_back(data, i);
	pwd_chunks = ft_split(pwd, '/');
	k = 0;
	while (pwd_chunks[k])
		k ++;
	k = k - sb;
	relative_pwd = join_chunks(pwd_chunks, "/", k);
	k = -1;
	while (pwd_chunks[++k])
		free(pwd_chunks[k]);
	free(pwd_chunks);
	free (pwd);
	return (relative_pwd);
}
