/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:13:13 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/02 16:39:29 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

extern char	**g_envs;

int	strs_to_fd(char **array, int fd)
{
	int	i;

	if (!array)
		return (-1);
	i = -1;
	while (array[++i])
	{
		if (write(fd, array[i], ft_strlen(array[i])) == -1)
			return (0);
		free(array[i]);
	}
	free(array);
	return (1);
}

int	handle_hd(t_ms *data, char *limit)
{
	int			fd;
	int			pid;
	char		**parse_d;
	char		*file_path;

	file_path = create_hd_fp();
	fd = open(file_path, O_CREAT | O_TRUNC | O_RDWR,
			S_IRWXU | S_IRWXG | S_IRWXO);
	pid = fork();
	if (!pid)
	{
		if (fd == -1)
			return (0);
		parse_d = parse_stdin_tolimit(limit);
		strs_to_fd(parse_d, fd);
		close(fd);
		exit_status(0, data, 0);
	}
	waitpid(pid, 0, 0);
	fd = open(file_path, O_RDONLY);
	free(file_path);
	return (fd);
}

int	*perform_hd_chain(t_ms *data)
{
	t_spl		*spl;
	static int	*hds;
	int			i;
	int			j;

	if (!data)
		return (hds);
	spl = get_cmdsplit(0);
	hds = ft_calloc(spl->cmd_count, sizeof(int));
	i = -1;
	while (spl->input_files && spl->input_files[++i])
	{
		j = -1;
		while (spl->input_files[i][++j])
		{
			if (!spl->input_files[i][j + 1] && spl->input_types[i][j])
				hds[i] = handle_hd(data, spl->input_files[i][j]);
			else if (spl->input_types[i][j])
				handle_hd(data, spl->input_files[i][j]);
		}
	}
	return (hds);
}
