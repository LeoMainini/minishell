/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:13:20 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/23 16:55:12 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*clean_path(char **dirty_path, char *extra)
{
	char	*eo_path;
	char	*c_path;
	long	size;

	eo_path = ft_strnstr(*dirty_path, extra, ft_strlen(*dirty_path));
	if (!eo_path)
	{
		check_free_zeroout((void **)&*dirty_path);
		return (0);
	}
	size = (long)eo_path - (long)*dirty_path;
	c_path = (char *)ft_calloc(size + 1, sizeof(char));
	if (!c_path)
		return (0);
	ft_memmove(c_path, *dirty_path, size);
	check_free_zeroout((void **)&*dirty_path);
	return (c_path);
}

char	*find_shell_path(char **envp)
{
	int		i;
	char	**all_paths;
	char	*output;
	char	*temp;

	i = -1;
	output = 0;
	while (envp[++i])
		if (!ft_strncmp(envp[i], "PATH", 4))
			all_paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (all_paths && all_paths[++i])
	{
		temp = ft_strfree_join(&all_paths[i], "/ls");
		if (!access(temp, F_OK | X_OK) && !output)
			output = ft_strdup(temp);
		if (temp)
			check_free_zeroout((void **)&temp);
	}
	if (all_paths)
		check_free_zeroout((void **)&all_paths);
	if (!output)
		return (0);
	return (clean_path(&output, "ls"));
}
