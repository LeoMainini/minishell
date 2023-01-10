/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/10 17:14:47 by leferrei         ###   ########.fr       */
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
		printf("%s has pid %d\n", spl->ss[i][0], pid);
		data->pids = save_pid(&(data->pids), pid, 0, data);
		if (!data->pids)
			return (0);
	}
	return (1);
}

char	*display_prompt(t_ms	*data)
{
	char	*prompt;
	char	*user_var;
	char	*user_name;
	char	*line;
	char	*ret_value_str;

	user_var = ft_strdup("USER");
	user_name = get_env_value(user_var, data);
	if (data->ret)
	{
		
		prompt = ft_strfree_join(&user_name, "\033[1;31m :: ");
		ret_value_str = ft_itoa(data->ret);
		if (ret_value_str)
			prompt = ft_strfree_join(&prompt, ret_value_str);
		free(ret_value_str);
		prompt = ft_strfree_join(&prompt, " \033[0;31m$> \033[0;37m");
	}
	else
		prompt = ft_strfree_join(&user_name, "\033[1;31m :: \033[0;31m$> \033[0;37m");
	line = readline(prompt);
	free(prompt);
	return (line);
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
	read_line = display_prompt(data);
	while (read_line)
	{
		status = 1;
		status = handle_exec_data(&read_line, data, &spl);
		i = -1;
		while (spl->ss && spl->ss[++i] && status != 2)
			status = execute_cmd(data, spl, i);
		if (!status && cleanup_exec_data(data, spl, &read_line))
			break ;
		if (status == 2 && cleanup_exec_data(data, spl, &read_line))
		{
			read_line = display_prompt(data);
			continue ;
		}
		await_pid_returns(data, data->pids, spl, i);
		cleanup_exec_data(data, spl, &read_line);
		read_line = display_prompt(data);
	}
	exit_status(0, data, 1);
}
