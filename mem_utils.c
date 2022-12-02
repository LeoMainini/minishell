/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:25:54 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/02 16:39:53 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include "lib-pipex/pipex.h"
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
			free((*files)[i][j]);
		free((*files)[i]);
		free((*types)[i]);
	}
	free((*files));
	(*files) = 0;
	free((*types));
}

void	free_cmdsplit(t_spl *cspl)
{
	int	i;
	int	j;

	i = -1;
	while (cspl->ss && cspl->ss[++i])
	{
		j = -1;
		while (cspl->ss[i][++j])
			free(cspl->ss[i][j]);
		free(cspl->ss[i]);
	}
	free(cspl->ss);
	cspl->ss = NULL;
	free_inout_strs(&cspl->input_files, &cspl->input_types);
	free_inout_strs(&cspl->output_files, &cspl->output_types);
	free(perform_hd_chain(0));
	free(cspl);
}

void	free_data(t_ms *data)
{
	int	i;

	if (data->rl_addr && *(data->rl_addr))
		free(*(data->rl_addr));
	i = -1;
	while (g_envs[++i])
		free(g_envs[i]);
	free(g_envs);
	rl_clear_history();
}
