/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 16:17:06 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/10 16:31:53 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

extern char	**g_envs;

void	select_builtin(int i, t_ms *data, t_cmdd *cmds, int nil)
{
	if (i == 1)
		change_dir(cmds, data, nil);
	if (i == 2)
		env(cmds, data);
	if (i == 3)
		pwd(cmds, data);
	if (i == 4)
		export(cmds, data, nil);
	if (i == 5)
		unset(cmds, data, nil);
	if (i == 6)
		exit_shell(cmds, data, nil);
	if (i == 7)
		echo(cmds, data);
}

int	execute_builtin(char ***cmd_argvs, int k, t_ms *data, int pip[2])
{
	int		i;
	int		redirs_status;
	t_cmdd	cmds;

	i = check_builtin(cmd_argvs[k][0]);
	if (!i)
		return (0);
	redirs_status = pre_exec_prep(cmd_argvs, k, pip, &cmds);
	if (redirs_status == -1 && !close(pip[1])
		&& ft_putstr_fd("No such file or directory\n", STDERR_FILENO))
		return (set_ret_return(data, 126));
	if (cmds.out_fd == STDOUT_FILENO && pip[0] > 1)
		close(pip[0]);
	select_builtin(i, data, &cmds, (cmd_argvs[k + 1] != 0));
	if (redirs_status <= 0 && pip[1] > 1)
		close(pip[1]);
	return (1);
}

void	exec_child_pid(int in_fd, int out_fd, int i, char ***cmd_argv)
{
	int		redirs_status;
	char	*exec_path;
	t_ms	*data;
	t_spl	*spl;

	signal(SIGINT, cmd_sighandler);
	spl = get_cmdsplit(0);
	data = get_struct(0);
	close(data->pip[0]);
	if (!cmd_argv[i][0])
		exit_status(0, data, 0);
	redirs_status = pre_sys_exec_prep(in_fd, out_fd, i, cmd_argv);
	if (redirs_status == -1
		&& ft_putstr_fd("No such file or directory\n", STDERR_FILENO))
		exit(126);
	exec_path = (get_executable_path(data, cmd_argv[i][0], g_envs));
	execve(exec_path, cmd_argv[i], g_envs);
	check_free_zeroout((void **)&exec_path);
	ft_putstr_fd("Error executing: ", STDERR_FILENO);
	ft_putendl_fd(cmd_argv[i][0], STDERR_FILENO);
	free_cmdsplit(spl, data);
	check_free_zeroout((void **)&data->pids);
	exit_status(127, data, 0);
}

int	exec_sys_func(char ***cmd_argv, int *i, int pip[2])
{
	int		pid;
	int		in_fd;
	int		out_fd;

	in_fd = pip[0];
	if (pipe(pip) == -1)
		return (-2);
	out_fd = pip[1];
	pid = fork();
	if (!pid)
		exec_child_pid(in_fd, out_fd, *i, cmd_argv);
	else
		close(pip[1]);
	return (pid);
}
