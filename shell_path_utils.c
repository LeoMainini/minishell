/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:13:20 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 15:35:42 by leferrei         ###   ########.fr       */
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
		free(*dirty_path);
		return (0);
	}
	size = (long)eo_path - (long)*dirty_path;
	c_path = (char *)ft_calloc(size + 1, sizeof(char));
	ft_memmove(c_path, *dirty_path, size);
	free(*dirty_path);
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
	while (all_paths[++i])
	{
		temp = ft_strfree_join(&all_paths[i], "/ls");
		if (!access(temp, F_OK | X_OK) && !output)
			output = ft_strdup(temp);
		if (temp)
			free(temp);
	}
	if (all_paths)
		free(all_paths);
	if (!output)
		return (0);
	return (clean_path(&output, "ls"));
}
