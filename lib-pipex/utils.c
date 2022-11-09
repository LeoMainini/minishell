/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:12:40 by leferrei          #+#    #+#             */
/*   Updated: 2022/07/27 15:12:42 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strfree_join(char **s1, char *s2)
{
	char	*result;

	result = ft_strjoin(*s1, s2);
	free(*s1);
	return (result);
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

void	free_and_exit(t_vars *data, int status)
{
	int	i;
	int	k;

	i = -1;
	while (data->cmds[++i])
	{
		k = -1;
		while (data->cmds[i][++k])
			free(data->cmds[i][k]);
		free(data->cmds[i]);
	}
	free(data->cmds);
	close(data->fds[0]);
	close(data->out_fd);
	i = -1;
	while (data->lines_in && data->lines_in[++i])
		free(data->lines_in[i]);
	if (data->lines_in)
		free(data->lines_in);
	if (data->path)
		free(data->path);
	if (data)
		free (data);
	exit (status);
}
