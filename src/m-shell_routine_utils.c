/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell_routine_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:21:34 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/06 15:48:50 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>

extern char	**g_envs;

int	*save_pid(int **pids, int new_pid, int reset)
{
	static int	k;
	int			i;
	int			*temp_pids;

	if (reset)
	{
		k = 0;
		return (0);
	}
	(*pids)[k] = new_pid;
	temp_pids = (int *)ft_calloc(k + 2, sizeof(int));
	i = -1;
	while (++i <= k)
		temp_pids[i] = (*pids)[i];
	k++;
	free(*pids);
	return (temp_pids);
}

void	split_inter(t_spl *spl, int i)
{
	char	**split_out;
	char	**result;
	int		k;
	int		j;

	split_out = ft_split(spl->ss[i][0], ' ');
	k = 0;
	while (split_out[k])
		k++;
	j = 0;
	while (spl->ss[i][j])
		j++;
	result = ft_calloc(j + k, sizeof(char *));
	k = -1;
	while (split_out[++k])
		result[k] = split_out[k];
	j = 0;
	while (spl->ss[i][++j])
		result[k + (j - 1)] = spl->ss[i][j];
	free(spl->ss[i][0]);
	free(spl->ss[i]);
	free(split_out);
	spl->ss[i] = result;
}

void	await_pid_returns(t_ms *data, int *pids, t_spl *spl, int i)
{
	int	j;
	int	status;

	j = 0;
	while (pids[j])
		j++;
	while (j > 0)
		waitpid(pids[--j], &status, 0);
	free(pids);
	if (spl->ss && spl->ss[0] && WIFSIGNALED(status)
		&& !check_builtin(spl->ss[i - 1][0]))
	{
		if (WTERMSIG(status) == SIGINT)
			data->ret = 130;
		if (WTERMSIG(status) == SIGQUIT)
			data->ret = 131;
	}
	else if (spl->ss && spl->ss[0] && !check_builtin(spl->ss[i - 1][0]))
		data->ret = WEXITSTATUS(status);
}

void	init_data(int argc, char **argv, t_ms **data, char **envp)
{
	(void)argc;
	(void)argv;
	*data = (t_ms *)ft_calloc(1, sizeof(t_ms));
	get_struct(data);
	(*data)->ret = 0;
	(*data)->builtins_outfd = -1;
	(*data)->system_outfd = -1;
	(*data)->rl_addr = 0;
	(*data)->in_child = 0;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp, 0, 0);
	(*data)->path = find_shell_path(g_envs);
}

int	handle_exec_data(char **read_line, t_ms *data, t_spl **spl)
{
	add_history(*read_line);
	data->rl_addr = read_line;
	*spl = set_cmdsplit(*read_line);
	get_cmdsplit(*spl);
	if (!*spl)
		return (0);
	perform_hd_chain(data);
	data->pip[0] = -1;
	data->pip[1] = -1;
	save_pid(0, 0, 1);
	data->pids = (int *)ft_calloc(1, sizeof(int));
	return (1);
}
