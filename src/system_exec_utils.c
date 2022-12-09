/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_exec_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:55:17 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 18:22:23 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

extern char	**g_envs;

int	handle_in(int i, t_spl *spl)
{
	int	fd;
	int	success;
	int	j;

	success = 0;
	j = -1;
	if (spl->input_files && spl->input_files[i][0])
	{
		while (spl->input_files[i][++j])
		{
			if (!spl->input_types[i][j])
				fd = open(spl->input_files[i][j], R_OK);
			if (fd == -1)
				success = -1;
			if (!spl->input_files[i][j + 1] && fd != -1)
				dup2(fd, STDIN_FILENO);
			if (fd > 1)
				close(fd);
		}
		if (success != -1)
			success = 1;
	}
	return (success);
}

int	handle_out(int i, t_spl *spl, int success)
{
	int	j;
	int	fd;

	j = -1;
	if (spl->output_files && spl->output_files[i][0])
	{
		while (spl->output_files[i][++j] && success != -1)
		{
			fd = open_outfile(spl, i, j);
			if (fd == -1)
				success = -1;
			if (!spl->output_files[i][j + 1] && fd != -1)
				dup2(fd, STDOUT_FILENO);
			if (fd >= 1)
				close(fd);
		}
		if (success == 1)
			return (3);
		else if (!success)
			return (2);
		else if (success == -1)
			return (-1);
	}
	return (success);
}

int	handle_redirections(int i)
{
	int		success;
	t_spl	*spl;
	int		fd;

	spl = get_cmdsplit(0);
	success = 0;
	fd = perform_hd_chain(0)[i];
	if (fd)
	{
		dup2(fd, STDIN_FILENO);
		success = 1;
	}
	else
		success = handle_in(i, spl);
	success = handle_out(i, spl, success);
	return (success);
}

int	pre_sys_exec_prep(int in_fd, int out_fd, int i, char ***cmd_argv)
{
	int	redirs_status;

	redirs_status = handle_redirections(i);
	if (in_fd > -1 && redirs_status != 1 && redirs_status != 3)
		dup2(in_fd, STDIN_FILENO);
	if (cmd_argv[i + 1] && redirs_status != 2 && redirs_status != 3)
		dup2(out_fd, STDOUT_FILENO);
	if (in_fd > 1)
		close(in_fd);
	close(out_fd);
	return (redirs_status);
}
