/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/02 16:39:39 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include "lib-pipex/pipex.h"
#include <sys/wait.h>
#include <unistd.h>

char	**g_envs;

void	execute_cmd(t_ms *data, t_spl *spl, int i)
{
	interpret_strings(spl->ss[i], data);
	split_inter(spl, i);
	if (!execute_builtin(spl->ss, i, data, data->pip))
		data->pids = save_pid(&(data->pids),
				exec_sys_func(spl->ss, &i, data->pip), 0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read_line;
	t_spl		*spl;
	t_ms		*data;
	int			i;

	init_data(argc, argv, &data, envp);
	read_line = readline("shell:> ");
	while (read_line)
	{
		if (!handle_exec_data(&read_line, data, &spl))
			break ;
		i = -1;
		while (spl->ss && spl->ss[++i])
			execute_cmd(data, spl, i);
		await_pid_returns(data, data->pids, spl, i);
		cleanup_exec_data(data, spl, &read_line);
		read_line = readline("shell:> ");
	}
	exit_status(0, data, 1);
}
