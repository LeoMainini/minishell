/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:25:54 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/18 16:03:10 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**g_envs;

void	free_inout_strs(char ****files, int ***types)
{
	int	i;
	int	j;

	i = -1;
	while ((*files) && (*files)[++i])
	{
		j = -1;
		while ((*files)[i][++j])
			check_free_zeroout((void **)&(*files)[i][j]);
		check_free_zeroout((void **)&(*files)[i]);
		check_free_zeroout((void **)&(*types)[i]);
	}
	check_free_zeroout((void **)&(*files));
	(*files) = 0;
	check_free_zeroout((void **)&(*types));
}

void	free_cmdsplit(t_spl *cspl, t_ms *data)
{
	int	i;
	int	j;

	i = -1;
	while (cspl->ss && cspl->ss[++i])
	{
		j = -1;
		while (cspl->ss[i][++j])
			check_free_zeroout((void **)&cspl->ss[i][j]);
		check_free_zeroout((void **)&cspl->ss[i]);
	}
	check_free_zeroout((void **)&cspl->ss);
	cspl->ss = NULL;
	free_inout_strs(&cspl->input_files, &cspl->input_types);
	free_inout_strs(&cspl->output_files, &cspl->output_types);
	check_free_zeroout((void **)&data->hds);
	check_free_zeroout((void **)&cspl);
}

void	free_data(t_ms *data)
{
	int	i;

	if (data->rl_addr && *(data->rl_addr))
		check_free_zeroout((void **)&*(data->rl_addr));
	i = -1;
	while (g_envs[++i])
		check_free_zeroout((void **)&g_envs[i]);
	check_free_zeroout((void **)&g_envs);
	check_free_zeroout((void **)&data->hds);
	check_free_zeroout((void **)&data->path);
	rl_clear_history();
	check_free_zeroout((void **)&data);
}

void	free_hd_pid_mem(t_ms *data, t_spl *spl, char *file_path)
{
	check_free_zeroout((void **)&file_path);
	free_cmdsplit(spl, data);
	exit_status(0, data, 0);
}

void	free_hd_subp_mem(t_ms *data, t_spl *spl, char **file_path)
{
	free_data(data);
	free_cmdsplit(spl, data);
	free(*file_path);
	exit(0);
}
