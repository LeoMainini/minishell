/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:13:13 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/17 17:26:43 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
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
		check_free_zeroout((void **)&array[i]);
	}
	check_free_zeroout((void **)&array);
	return (1);
}

void	hd_process_routine(int fd, char *limit, t_ms *data, char *file_path)
{
	char	**parse_d;
	t_spl	*spl;

	signal(SIGINT, cmd_sighandler);
	spl = get_cmdsplit(0);
	if (fd == -1)
		exit(0);
	parse_d = parse_stdin_tolimit(limit);
	if (!parse_d)
		return (free_hd_subp_mem(data, spl, &file_path));
	strs_to_fd(parse_d, fd);
	close(fd);
	free_hd_pid_mem(data, spl, file_path);
}

int	handle_hd(t_ms *data, char *limit)
{
	int		fd;
	int		pid;
	char	*file_path;
	int		status;

	file_path = save_get_fp(create_hd_fp());
	fd = open(file_path, O_CREAT | O_TRUNC | O_RDWR,
			S_IRWXU | S_IRWXG | S_IRWXO);
	pid = fork();
	if (!file_path || pid == -1)
		return (0);
	if (!pid)
		hd_process_routine(fd, limit, data, file_path);
	waitpid(pid, (int *)&status, 0);
	if (status == 256 * 20)
		return (-1);
	fd = open(file_path, O_RDONLY);
	check_free_zeroout((void **)&file_path);
	return (fd);
}

int	perform_cmd_hds(t_ms *data, int **hds, int i)
{
	int		ret;
	t_spl	*spl;
	int		j;

	ret = 0;
	j = -1;
	spl = get_cmdsplit(0);
	while (spl->input_files[i][++j])
	{
		if (!spl->input_files[i][j + 1] && spl->input_types[i][j])
			(*hds)[i] = handle_hd(data, spl->input_files[i][j]);
		else if (spl->input_types[i][j])
			ret = handle_hd(data, spl->input_files[i][j]);
		if (ret == -1 || (*hds)[i] == -1)
			return (0);
	}
	return (1);
}

int	*perform_hd_chain(t_ms *data)
{
	t_spl		*spl;
	static int	*hds;
	int			i;

	if (!data)
		return (hds);
	spl = get_cmdsplit(0);
	hds = ft_calloc(spl->cmd_count, sizeof(int));
	if (!hds)
		return (0);
	data->hds = hds;
	i = -1;
	while (spl->input_files && spl->input_files[++i])
		if (!perform_cmd_hds(data, &hds, i))
			return ((int *)2);
	return (hds);
}
