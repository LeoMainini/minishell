/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_processing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:12:30 by leferrei          #+#    #+#             */
/*   Updated: 2022/07/27 15:12:32 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_cmd_error(char ***cmds, int i, int argc)
{
	int	k;

	if (!cmds[i - 2][0])
	{
		i = 1;
		while (++i < argc - 1 && cmds[i - 2])
		{
			k = -1;
			while (cmds[i - 2][++k])
				free(cmds[i - 2][k]);
			free(cmds[i - 2]);
		}
		free(cmds);
		return (1);
	}
	return (0);
}

char	***get_commands(int argc, char **argv)
{
	char	***cmds;
	int		i;

	i = 2;
	while (i < argc - 1)
		++i;
	cmds = (char ***)ft_calloc(i - 1, sizeof(char *));
	if (!cmds)
		return (0);
	i = 1;
	while (++i < argc - 1)
	{
		cmds[i - 2] = ft_split(argv[i], ' ');
		if (check_cmd_error(cmds, i, argc))
			return (0);
	}
	cmds[i - 2] = 0;
	return (cmds);
}

char	***get_hd_commands(int argc, char **argv)
{
	char	***cmds;
	int		i;

	i = 3;
	while (i < argc - 1)
		++i;
	cmds = (char ***)ft_calloc(i - 1, sizeof(char *));
	if (!cmds)
		return (0);
	i = 2;
	while (++i < argc - 1)
	{
		cmds[i - 3] = ft_split(argv[i], ' ');
		if (check_cmd_error(cmds, i - 1, argc))
			return (0);
	}
	cmds[i - 3] = 0;
	return (cmds);
}

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

char	**append_item(char **src, char *entry)
{
	int		i;
	int		size;
	char	**output;

	i = -1;
	size = 0;
	while (src[++i] != NULL)
		size += ft_strlen(src[i]);
	output = (char **)ft_calloc(size + ((i + 1) * 8), sizeof(char ));
	i = -1;
	while (src[++i] != NULL)
	{
		output[i] = ft_strdup(src[i]);
		free(src[i]);
	}
	free(src[i]);
	free(src);
	output[i] = ft_strdup(entry);
	output[i + 1] = 0;
	return (output);
}
