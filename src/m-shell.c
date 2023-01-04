/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/04 17:37:57 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

char	**g_envs;

int	execute_cmd(t_ms *data, t_spl *spl, int i)
{
	int	pid;
	
	interpret_strings(spl->ss[i], data);
	if (!split_inter(spl, i)
		&& ft_putstr_fd("Error expanding command value\n", STDERR_FILENO))
		return (0);
	if (!execute_builtin(spl->ss, i, data, data->pip))
	{
		pid = exec_sys_func(spl->ss, &i, data->pip);
		// ft_putstr_fd("NEW PID = ", STDERR_FILENO);
		// ft_putendl_fd(ft_itoa(pid), STDERR_FILENO);
		data->pids = save_pid(&(data->pids), pid , 0, data);
		if (!data->pids)
			return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read_line;
	t_spl		*spl;
	t_ms		*data;
	int			i;
	int			status;

	if (!init_data(argc, argv, &data, envp))
		return (0);
	read_line = readline("shell:> ");
	while (read_line)
	{
		if (!handle_exec_data(&read_line, data, &spl))
			break ;
		i = -1;
		while (spl->ss && spl->ss[++i])
			status = execute_cmd(data, spl, i);
		if (!status && cleanup_exec_data(data, spl, &read_line))
			break ;
		await_pid_returns(data, data->pids, spl, i);
		cleanup_exec_data(data, spl, &read_line);
		read_line = readline("shell:> ");
	}
	exit_status(0, data, 1);
}
