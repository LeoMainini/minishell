/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell_routine_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:21:34 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/23 16:54:41 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>

extern char	**g_envs;

int	*save_pid(int **pids, int new_pid, int reset, t_ms *data)
{
	static int	k;
	int			i;
	int			*temp_pids;

	if (reset)
	{
		k = 0;
		return (0);
	}
	data->pids_written = 1;
	(*pids)[k] = new_pid;
	temp_pids = (int *)ft_calloc(k + 2, sizeof(int));
	if (!temp_pids || new_pid == -1)
		return (0);
	i = -1;
	while (++i <= k)
		temp_pids[i] = (*pids)[i];
	k++;
	check_free_zeroout((void **)&*pids);
	return (temp_pids);
}

int	split_inter(t_spl *spl, int i)
{
	char	**split_out;
	char	**result;
	int		k;
	int		j;

	split_out = ft_split(spl->ss[i][0], ' ');
	if (!split_out)
		return (0);
	result = alloc_result_mem(spl, i, split_out);
	if (!result)
	{
		free_strarray(split_out);
		free_split_inter_data(spl, split_out, i, 0);
		return (0);
	}
	k = -1;
	while (split_out[++k])
		result[k] = split_out[k];
	j = 0;
	while (spl->ss[i][++j])
		result[k + (j - 1)] = spl->ss[i][j];
	free_split_inter_data(spl, split_out, i, 1);
	spl->ss[i] = result;
	return (1);
}

void	await_pid_returns(t_ms *data, int *pids, t_spl *spl, int i)
{
	int	j;
	int	status;

	j = 0;
	status = 0;
	while (pids[j])
		j++;
	if (j > 0)
		waitpid(pids[--j], &status, 0);
	while (j > 0)
		waitpid(pids[--j], 0, 0);
	check_free_zeroout((void **)&pids);
	data->pids_written = 0;
	if (spl->ss && spl->ss[0] && WIFSIGNALED(status)
		&& !check_builtin(spl->ss[i - 1][0]))
	{
		if (WTERMSIG(status) == SIGINT && printf("\n"))
			data->ret = 130;
		if (WTERMSIG(status) == SIGQUIT && printf("\n"))
			data->ret = 131;
	}
	else if (spl->ss && spl->ss[0] && !check_builtin(spl->ss[i - 1][0]))
		data->ret = WEXITSTATUS(status);
}

int	init_data(t_ms **data, char **argv, char **envp)
{
	(void)argv;
	*data = (t_ms *)ft_calloc(1, sizeof(t_ms));
	if (!*data)
		return (0);
	get_struct(data);
	(*data)->ret = 0;
	(*data)->builtins_outfd = -1;
	(*data)->system_outfd = -1;
	(*data)->rl_addr = 0;
	(*data)->in_child = 0;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, SIG_IGN);
	g_envs = duplicate_envp(envp, 0, 0);
	if (!g_envs)
	{
		check_free_zeroout((void **)data);
		return (0);
	}
	(*data)->path = find_shell_path(g_envs);
	if (!(*data)->path)
	{
		check_free_zeroout((void **)data);
		return (0);
	}
	return (1);
}
