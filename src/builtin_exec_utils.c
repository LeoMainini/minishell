/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 14:55:05 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 18:27:04 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**g_envs;

int	check_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!scmp(cmd, "cd"))
		return (1);
	if (!scmp(cmd, "env"))
		return (2);
	if (!scmp(cmd, "pwd"))
		return (3);
	if (!scmp(cmd, "export"))
		return (4);
	if (!scmp(cmd, "unset"))
		return (5);
	if (!scmp(cmd, "exit"))
		return (6);
	if (!scmp(cmd, "echo"))
		return (7);
	return (0);
}

int	handle_b_in(int i, t_spl *spl)
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
		}
	}
	return (success);
}

int	handle_b_out(int i, t_spl *spl, int success)
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
			if (!spl->output_files[i][j + 1] && fd != -1 && success > -1)
				success = fd;
			else
				close(fd);
		}
	}
	return (success);
}

int	handle_b_redirections(int i)
{
	int		success;
	t_spl	*spl;
	int		fd;

	spl = get_cmdsplit(0);
	success = 0;
	fd = perform_hd_chain(0)[i];
	if (!fd)
		success = handle_b_in(i, spl);
	success = handle_b_out(i, spl, success);
	return (success);
}

int	pre_exec_prep(char ***cmd_argvs, int k, int pip[2], t_cmdd *cmds)
{
	int	redirs_status;

	(*cmds).args = cmd_argvs[k];
	(*cmds).in_fd = -1;
	if (pip[0] > 1)
		close(pip[0]);
	if (pipe(pip) == -1)
		return (-1);
	redirs_status = handle_b_redirections(k);
	if (cmd_argvs[k + 1] != 0 && redirs_status <= 0)
		(*cmds).out_fd = pip[1];
	else if (redirs_status <= 0)
		(*cmds).out_fd = STDOUT_FILENO;
	else
	{
		close(pip[1]);
		(*cmds).out_fd = redirs_status;
	}
	return (redirs_status);
}
