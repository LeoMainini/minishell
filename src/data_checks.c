/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 16:34:06 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/21 16:43:14 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>

int	check_file_existing(char *cmd)
{
	char	*temp;

	if (access(cmd, F_OK) && ft_putstr_fd("File not found\n", STDERR_FILENO))
		return (0);
	temp = ft_strjoin(cmd, "/.");
	if (!access(temp, F_OK) && ft_putstr_fd("File is a folder\n", STDERR_FILENO))
	{
		check_free_zeroout((void **)&temp);
		return (0);
	}
	check_free_zeroout((void **)&temp);
	return (1);
}

int	handle_exec_data(char **read_line, t_ms *data, t_spl **spl)
{
	int	*hd_status;

	add_history(*read_line);
	data->rl_addr = read_line;
	*spl = set_cmdsplit(*read_line);
	get_cmdsplit(*spl);
	hd_status = perform_hd_chain(data);
	data->pip[0] = -1;
	data->pip[1] = -1;
	save_pid(0, 0, 1, data);
	data->pids = (int *)ft_calloc(1, sizeof(int));
	if (!data->pids || !*spl || hd_status == (int *)2 || !hd_status)
	{
		if (hd_status == (int *)2)
			data->ret = 130;
		if (hd_status && hd_status > (int *)2)
			check_free_zeroout((void **)&hd_status);
		check_free_zeroout((void **)&data->pids);
		return (2);
	}
	data->pids_written = 0;
	return (1);
}
