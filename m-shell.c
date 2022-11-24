/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/24 17:17:18 by leferrei         ###   ########.fr       */
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

char **g_envs;

void exit_status(int status, char **line)
{
	int	i;

	free(*line);
	i = -1;
	while (g_envs[++i])
		free(g_envs[i]);
	free(g_envs);
	rl_clear_history();
	exit(status);
}

char	**alloc_envmem(char **envs, int	offset)
{
	int	i;
	char **temp;

	if (!envs)
		return (0);
	i = 0;
	while (envs[i])
		i++;
	temp = (char **)ft_calloc(i + offset + 1, sizeof(char *));
	if (!temp)
		return (0);
	return (temp);
}

char	**duplicate_envp(char **envs, int offset)
{
	int		i;
	char	**temp;

	if (!envs)
		return (0);
	i = 0;
	while (envs[i])
		i++;
	temp = (char **)ft_calloc(i + offset + 1, sizeof(char *));
	if (!temp)
		return (0);
	while (i--)
		temp[i] = ft_strdup(envs[i]);
	return (temp);
}

int	check_envp_duplicate_error(char **envs)
{
	int	i;

	i = -1;
	while (envs[++i])
		if (ft_strcmp(envs[i], g_envs[i]))
			return (i);
	return (0);
}

int	check_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (2);
	if (!ft_strcmp(cmd, "pwd"))
		return (3);
	if (!ft_strcmp(cmd, "export"))
		return (4);
	if (!ft_strcmp(cmd, "unset"))
		return (5);
	if (!ft_strcmp(cmd, "exit"))
		return (6);
	if (!ft_strcmp(cmd, "echo"))
		return (7);
	return (0);
}

int execute_builtin(char ***cmd_argvs, int k, t_ms *data, int pip[2])
{
	int	i;
	t_cmdd		cmds;

	i = check_builtin(cmd_argvs[k][0]);
	if (!i)
		return (0);
	cmds.args = cmd_argvs[k];
	cmds.in_fd = -1;
	close(pip[0]);
	if (pipe(pip) == -1)
		return (-1);
	data->builtins_outfd = pip[0];
	if (cmd_argvs[k + 1] != 0)
		cmds.out_fd = pip[1];
	else
		cmds.out_fd = STDOUT_FILENO;
	if (i == 1)
		change_dir(&cmds, data, cmd_argvs[k + 1] != 0);
	if (i == 2)
		env(&cmds, data);
	if (i == 3)
		pwd(&cmds, data);
	if (i == 4)
		export(&cmds, data, cmd_argvs[k + 1] != 0);
	if (i == 5)
		unset(&cmds, data, cmd_argvs[k + 1] != 0);
	if (i == 6)
		exit_shell(&cmds, data, cmd_argvs[k + 1] != 0);
	if (i == 7)
		echo(&cmds, data);
	close(pip[1]);
	if (cmds.out_fd == STDOUT_FILENO)
		close(pip[0]);
	return (1);
}

int	check_cmd_executable(char *cmd)
{
	char *temp;

	if (!strchr(cmd, '/'))
		return (0);
	if (access(cmd, F_OK) && ft_putstr_fd("File not found\n", STDERR_FILENO))
		return (0);
	if ((access(cmd, X_OK)) && ft_putstr_fd("File not executable\n", STDERR_FILENO))
		return (0);
	temp = ft_strjoin(cmd, "/.");
	if (!access(temp, F_OK) && ft_putstr_fd("File is a folder\n", STDERR_FILENO))
	{
		free(temp);
		return (0);
	}
	free(temp);
	if (!ft_strncmp(cmd, "..", 2)
		|| !ft_strncmp(cmd, "./", 2))
		return (2);
	return (1);
}

char *get_executable_path(t_ms *data, char *cmd, char **envp)
{
	(void)envp;
	if (check_cmd_executable(cmd))
		return (ft_strdup(cmd));
	return (ft_strjoin(data->path, cmd));
}

int	exec_sys_func(char*** cmd_argv, int *i, t_ms *data, int pip[2])
{
	int		pid;
	int		in_fd;
	int		out_fd;
	char	*exec_path;

	in_fd = pip[0];
	if (pipe(pip) == -1)
		return -2;
	out_fd = pip[1];
	pid = fork();
	if (!pid)
	{
		if (in_fd > -1)
			dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		if(cmd_argv[*i + 1])
			dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
		exec_path = (get_executable_path(data, cmd_argv[*i][0], g_envs));
		execve(exec_path, cmd_argv[*i], g_envs);
		free(exec_path);
		ft_putstr_fd("Error executing: ", STDERR_FILENO);
		ft_putendl_fd(cmd_argv[*i][0], STDERR_FILENO);
		exit(127);
	}
	else
		close(pip[1]);
	return (pid);
}

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
	free(*pids);
	return (temp_pids);
}

// static void free_spl_redirs(t_spl *cspl)
// {
// 	int	i;

// 	i = -1;
// 	while (cspl->input_files && cspl->input_files[++i])
// 		free(cspl->input_files[i]);
// 	free(cspl->input_files);
// 	free(cspl->input_types);
// 	i = -1;
// 	while (cspl->output_files && cspl->output_files[++i])
// 		free(cspl->output_files[i]);
// 	free(cspl->output_files);
// 	free(cspl->output_types);
// }

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
	//free_spl_redirs(cspl);
	free(cspl);
}

t_ms *get_struct(t_ms **data)
{
	static t_ms *result;

	if (data)
		result = *data;
	return (result);
}

t_spl *get_cmdsplit(char *read_line)
{
	t_spl *temp;
	temp = (t_spl *)ft_calloc(1, sizeof(t_spl));
	
	if (!temp)
		return (0);
	*temp = cmd_split(read_line);
	return (temp);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read_line;
	t_spl		*spl;
	t_ms		*data;
	int			i;
	int			pip[2];
	int			*pids;
	int			j;

	(void)argc;
	(void)argv;
	data = (t_ms *)ft_calloc(1, sizeof(t_ms));
	get_struct(&data);
	data->ret = 0;
	data->builtins_outfd = -1;
	data->system_outfd = -1;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp, 0);
	data->path = find_shell_path(g_envs);
	read_line = readline("shell:> ");
	while (read_line)
	{
		add_history(read_line);
		data->rl_addr = &read_line;
		spl = get_cmdsplit(read_line);
		if (!spl)
			break ;
		pip[0] = -1;
		pip[1] = -1;
		i = -1;
		save_pid(0, 0, 1);
		pids = (int *)ft_calloc(1, sizeof(int));
		while (spl->ss && spl->ss[++i])
		{
			interpret_strings(spl->ss[i], data);
			if (!execute_builtin(spl->ss, i, data, pip))
				pids = save_pid(&pids, exec_sys_func(spl->ss, &i, data, pip), 0);
		}
		j = -1;
		while(pids[++j])
			waitpid(pids[j], &data->ret, 0);
		free(pids);		
		if (WIFSIGNALED(data->ret))
		{
			if (WTERMSIG(data->ret) == 2)
				data->ret = 130;
		}
		else if (WIFSTOPPED(data->ret))
			data->ret = WSTOPSIG(data->ret);
		else
			data->ret = WEXITSTATUS(data->ret);
		printf("last exit status int %d\n", data->ret);
		close(pip[0]);
		free_cmdsplit(spl);
		free(read_line);
		read_line = readline("shell:> ");
	}
	ft_putendl_fd("exit", STDERR_FILENO);
	exit_status(0, &read_line);
}
