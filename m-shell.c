/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/30 16:48:13 by leferrei         ###   ########.fr       */
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

void free_data(t_ms *data)
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

void exit_status(int status, t_ms *data)
{
	free_data(data);
	ft_putendl_fd("exit", STDERR_FILENO);
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

char	**duplicate_envp(char **envs, int offset, int freeable)
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
	{
		temp[i] = ft_strdup(envs[i]);
		if (freeable)
			free(envs[i]);
	}
	return (temp);
}

int	check_envp_duplicate_error(char **envs)
{
	int	i;

	i = -1;
	while (envs[++i])
		if (scmp(envs[i], g_envs[i]))
			return (i);
	return (0);
}

int	check_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!scmp(cmd, "cd"))
		return (1);
	if (!scmp(cmd, "env"))
		return (2);
	if (!scmp(cmd, "pwd"))
		return (3);
	if (!scmp(cmd, "export"))
		return (4);
	if (!scmp(cmd, "unset"))
		return (5);
	if (!scmp(cmd, "exit"))
		return (6);
	if (!scmp(cmd, "echo"))
		return (7);
	return (0);
}

int	handle_b_in(int i, t_spl *spl)
{
	int	fd;
	int	success;
	int	j;

	success = 0;
	j = -1;
	if (spl->input_files && spl->input_files[i][0])
	{
		while (spl->input_files[i][++j])
		{
			if (!spl->input_types[i][j])
				fd = open(spl->input_files[i][j], R_OK);
			if (fd == -1)
				success = -1;
		}
	}
	return (success);
}

int	open_outfile(t_spl *spl, int i, int j)
{
	int	fd;

	if (!spl->output_types[i][j])
		fd = open(spl->output_files[i][j], O_RDWR | O_TRUNC | O_CREAT);
	else if (spl->output_types[i][j] == 1)
		fd = open(spl->output_files[i][j], O_APPEND | O_RDWR | O_CREAT);
	return (fd);
}

int	handle_b_out(int i, t_spl *spl, int success)
{
	int	j;
	int	fd;

	j = -1;	
	if (spl->output_files && spl->output_files[i][0])
	{
		while (spl->output_files[i][++j] && success != -1)
		{
				fd = open_outfile(spl, i, j);
			if (fd == -1)
				success = -1;
			if (!spl->output_files[i][j + 1] && fd != -1 && success > -1)
				success = fd;
			else
				close(fd);
		}
	}
	return (success);
}

int	handle_b_redirections(int	i)
{
	int		success;
	t_spl	*spl;
	int		fd;

	spl = fetch_cmdsplit(0);
	success = 0;
	fd = perform_hd_chain(0)[i];
	if(!fd)
		success = handle_b_in(i, spl);
	success = handle_b_out(i, spl, success);
	return (success);
}

int execute_builtin(char ***cmd_argvs, int k, t_ms *data, int pip[2])
{
	int	i;
	int	redirs_status;
	t_cmdd		cmds;

	i = check_builtin(cmd_argvs[k][0]);
	if (!i)
		return (0);
	cmds.args = cmd_argvs[k];
	cmds.in_fd = -1;
	close(pip[0]);
	if (pipe(pip) == -1)
		return (-1);
	redirs_status = handle_b_redirections(k);
	if (cmd_argvs[k + 1] != 0 && redirs_status <= 0)
		cmds.out_fd = pip[1];
	else if (redirs_status <= 0)
		cmds.out_fd = STDOUT_FILENO;
	else
	{
		close(pip[1]);
		cmds.out_fd = redirs_status;
	}
	if (redirs_status == -1 && !close(pip[1])
		&& ft_putstr_fd("No such file or directory\n", STDERR_FILENO))
		return(set_ret_return(data, 126));
		//change all sub process exits to free_exit
	if (cmds.out_fd == STDOUT_FILENO)
		close(pip[0]);
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
	if (redirs_status <= 0)
		close(pip[1]);
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

int	strs_to_fd(char **array, int fd)
{
	int	i;

	if (!array)
		return (-1);
	i = -1;
	while (array[++i])
	{
		if (write(fd, array[i], ft_strlen(array[i])) == -1)
			return (0);
		free(array[i]);
	}
	free(array);
	return (1);
}

int	handle_hd(t_ms *data, char *limit)
{
	int			fd;
	int			pid;
	char		**parse_d;
	char		*file_path;
	static int	i;

	file_path = ft_strjoin("/tmp/hd_data", ft_itoa(i++));
	fd = open(file_path, O_CREAT | O_TRUNC | O_RDWR,
		S_IRWXU | S_IRWXG | S_IRWXO);
	pid = fork();
	if (!pid)
	{
		if (fd == -1)
			return (0);
		parse_d = parse_stdin_tolimit(limit);
		strs_to_fd(parse_d, fd);
		close(fd);
		exit_status(0, data);
	}
	waitpid(pid, 0, 0);
	fd = open(file_path, O_RDONLY);
	free(file_path);
	return (fd);
}

int	*perform_hd_chain(t_ms *data)
{
	t_spl		*spl;
	static int	*hds;
	int			i;
	int			j;

	if (!data)
		return (hds);
	spl = fetch_cmdsplit(0);
	hds = ft_calloc(spl->cmd_count, sizeof(int));
	i = -1;
	while (spl->input_files && spl->input_files[++i])
	{
		j = -1;
		while (spl->input_files[i][++j])
		{
			if (!spl->input_files[i][j + 1] && spl->input_types[i][j])
				hds[i] = handle_hd(data, spl->input_files[i][j]);
			else if (spl->input_types[i][j])
				handle_hd(data, spl->input_files[i][j]);
		}
	}
	return (hds);
}

int	handle_in(int i, t_spl *spl)
{
	int	fd;
	int	success;
	int	j;

	success = 0;
	j = -1;
	if (spl->input_files && spl->input_files[i][0])
	{
		while (spl->input_files[i][++j])
		{
			if (!spl->input_types[i][j])
				fd = open(spl->input_files[i][j], R_OK);
			if (fd == -1)
				success = -1;
			if (!spl->input_files[i][j + 1] && fd != -1)
				dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (success != -1)
			success = 1;
	}
	return (success);
}

int	handle_out(int i, t_spl *spl, int success)
{
	int	j;
	int	fd;

	j = -1;	
	if (spl->output_files && spl->output_files[i][0])
	{
		while (spl->output_files[i][++j] && success != -1)
		{
			fd = open_outfile(spl, i, j);
			if (fd == -1)
				success = -1;
			if (!spl->output_files[i][j + 1] && fd != -1)
				dup2(fd, STDOUT_FILENO);
			if (fd >= 0)
				close(fd);
		}
		if (success == 1)
			return (3);
		else if (!success)
			return (2);
		else if (fd == -1)
			return (-1);
	}
	return (success);
}

int	handle_redirections(int	i, int pip[2])
{
	int		success;
	t_spl	*spl;
	int		fd;

	spl = fetch_cmdsplit(0);
	success = 0;
	fd = perform_hd_chain(0)[i];
	if(fd)
	{
		dup2(fd, STDIN_FILENO);
		success = 1;
		close(pip[0]);
		close(pip[1]);
	}
	else
		success = handle_in(i, spl);
	success = handle_out(i, spl, success);
	return (success);
}

int	exec_sys_func(char*** cmd_argv, int *i, t_ms *data, int pip[2])
{
	int		pid;
	int		in_fd;
	int		out_fd;
	char	*exec_path;
	int		redirs_status;

	in_fd = pip[0];
	if (pipe(pip) == -1)
		return -2;
	out_fd = pip[1];
	redirs_status = 0;
	pid = fork();
	if (!pid)
	{
		redirs_status = handle_redirections(*i, pip);
		if (in_fd > -1 && redirs_status != 1 && redirs_status != 3)
			dup2(in_fd, STDIN_FILENO);
		if(cmd_argv[*i + 1] && redirs_status != 2 && redirs_status != 3 )
			dup2(out_fd, STDOUT_FILENO);
		close(in_fd);
		close(out_fd);
		if (redirs_status == -1
			&& ft_putstr_fd("No such file or directory\n", STDERR_FILENO))
			exit(126);
		if (!cmd_argv[*i][0])
			exit_status(0, data);
		exec_path = (get_executable_path(data, cmd_argv[*i][0], g_envs));
		execve(exec_path, cmd_argv[*i], g_envs);
		free(exec_path);
		ft_putstr_fd("Error executing: ", STDERR_FILENO);
		ft_putendl_fd(cmd_argv[*i][0], STDERR_FILENO);
		exit_status(127, data);	
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
	printf("new pid = %d\n", new_pid);
	temp_pids = (int *)ft_calloc(k + 2, sizeof(int));
	i = -1;
	while (++i <= k)
		temp_pids[i] = (*pids)[i];
	k++;
	free(*pids);
	return (temp_pids);
}

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

// static void free_spl_redirs(t_spl *cspl)
// {
// 	int	i;
// 	int	j;

// 	i = -1;
// 	while (cspl->input_files && cspl->input_files[++i])
// 	{
// 		j = -1;
// 		while (cspl->input_files[i][++j])
// 			free(cspl->input_files[i][j]);
// 		free(cspl->input_files[i]);
// 		free(cspl->input_types[i]);
// 	}
// 	free(cspl->input_files);
// 	cspl->input_files = 0;
// 	free(cspl->input_types);
// 	i = -1;
// 	while (cspl->output_files && cspl->output_files[++i])
// 	{
// 		j = -1;
// 		while (cspl->output_files[i][++j])
// 			free(cspl->output_files[i][j]);
// 		free(cspl->output_files[i]);
// 		free(cspl->output_types[i]);
// 	}
// 	free(cspl->output_files);
// 	cspl->output_files = 0;
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
	free_inout_strs(&cspl->input_files, &cspl->input_types);
	free_inout_strs(&cspl->output_files, &cspl->output_types);
	free(perform_hd_chain(0));
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

t_spl	*fetch_cmdsplit(t_spl *cmdsplit)
{
	static t_spl	*temp;
	
	if (cmdsplit)
		temp = cmdsplit;
	return (temp);
}

void split_inter(t_spl *spl, int i)
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
	while(pids[j])
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
		*spl = get_cmdsplit(*read_line);
		fetch_cmdsplit(*spl);
		if (!*spl)
			return (0);
		perform_hd_chain(data);
		data->pip[0] = -1;
		data->pip[1] = -1;
		save_pid(0, 0, 1);
		data->pids = (int *)ft_calloc(1, sizeof(int));
		return (1);
}

void	execute_cmd(t_ms *data, t_spl *spl, int i)
{
	interpret_strings(spl->ss[i], data);
	split_inter(spl, i);
	if (!execute_builtin(spl->ss, i, data, data->pip))
		data->pids = save_pid(&(data->pids),
				exec_sys_func(spl->ss, &i, data, data->pip), 0);
}

void	cleanup_exec_data(t_ms *data, t_spl *spl, char **read_line)
{
		close(data->pip[0]);
		free_cmdsplit(spl);
		free(*read_line);
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
	exit_status(0, data);
}
