/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:04:40 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 17:50:19 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**g_envs;

int	open_outfile(t_spl *spl, int i, int j)
{
	int	fd;

	if (!spl->output_types[i][j])
		fd = open(spl->output_files[i][j], O_RDWR | O_TRUNC | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else if (spl->output_types[i][j] == 1)
		fd = open(spl->output_files[i][j], O_CREAT | O_APPEND | O_RDWR,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return (fd);
}

int	check_cmd_executable(char *cmd)
{
	char	*temp;

	if (!ft_strchr(cmd, '/'))
		return (0);
	if (access(cmd, F_OK) && ft_putstr_fd("File not found\n", STDERR_FILENO))
		return (0);
	if ((access(cmd, X_OK))
		&& ft_putstr_fd("File not executable\n", STDERR_FILENO))
		return (0);
	temp = ft_strjoin(cmd, "/.");
	if (!access(temp, F_OK) && ft_putstr_fd("File is a folder\n", STDERR_FILENO))
	{
		check_free_zeroout((void **)&temp);
		return (0);
	}
	check_free_zeroout((void **)&temp);
	if (!ft_strncmp(cmd, "..", 2)
		|| !ft_strncmp(cmd, "./", 2))
		return (2);
	return (1);
}

char	*get_executable_path(t_ms *data, char *cmd, char **envp)
{
	(void)envp;
	if (check_cmd_executable(cmd))
		return (ft_strdup(cmd));
	return (ft_strjoin(data->path, cmd));
}

char	*create_hd_fp(void)
{
	char		*char_i;
	static int	i;
	char		*temp;

	char_i = ft_itoa(i++);
	temp = ft_strjoin("/tmp/hd_data", char_i);
	check_free_zeroout((void **)&char_i);
	return (temp);
}
