/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/28 17:43:08 by leferrei         ###   ########.fr       */
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

void exit_status(int status, t_ms *data)
{
	int	i;

	if (data->rl_addr && *(data->rl_addr))
		free(*(data->rl_addr));
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
		if (ft_strcmp(envs[i], g_envs[i]))
			return (i);
	return (0);
}

int	check_builtin(char *cmd)
{
	if (!cmd)
		return (0);
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
int	handle_b_redirections(int	i)
{
	int		success;
	int		fd;
	t_spl	*spl;
	int		j;

	printf("i = %d\n", i);
	spl = fetch_cmdsplit(0);
	success = 0;
	j = -1;
	if (spl->input_files && spl->input_files[i][0])
		while (spl->input_files[i][++j])
		{
			printf("input file = %s\n", spl->input_files[i][j]);
			fd = open(spl->input_files[i][j], R_OK);	
			if (fd == -1)
				success = -1;
			close(fd);
		}
	j = -1;
	if (spl->output_files && spl->output_files[i][0])
	{
		while (spl->output_files[i][++j])
		{
			printf("output file = %s in mode %d\n", spl->output_files[i][j], spl->output_types[i][j]);
			//test file opening for directory before opening if trying this fails
			if (!spl->output_types[i][j])
				fd = open(spl->output_files[i][j], O_RDWR | O_TRUNC | O_CREAT);
			else if (spl->output_types[i][j] == 1)
				fd = open(spl->output_files[i][j], O_APPEND | O_RDWR | O_CREAT);
			if (fd == -1)
				success = -1;
			if (spl->input_files[i][j + 1] && fd != -1)
				close(fd);
		}
		if (success != -1)
			success = fd;
	}
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

int	handle_redirections(int	i, t_ms *data, int pip[2])
{
	int		success;
	int		fd;
	t_spl	*spl;
	int		j;
	int		pid;
	int		status;

	spl = fetch_cmdsplit(0);
	success = 0;
	j = -1;
	//int	log_fd =  open(ft_strjoin(ft_itoa(i), "-logfile.txt"), O_CREAT | O_RDWR | O_TRUNC);
	//redir input files
	j = -1;
	if (spl->input_files && spl->input_files[i][0])
	{
		while (spl->input_files[i][++j])
		{
			printf("input file = %s\n", spl->input_files[i][j]);
			if (!spl->input_types[i][j])
				fd = open(spl->input_files[i][j], R_OK);
			else if (spl->input_types[i][j] == 1)
			{
				fd = open("/tmp/hd_data", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
				pid = fork();
				if (!pid)
				{
					close(pip[1]);
					close(pip[0]);
					if (fd == -1)
					{
						ft_putendl_fd("couldnt open fd", STDERR_FILENO);
						success = -1;
					}
					else
					{
						ft_putendl_fd("opened fd", STDERR_FILENO);
						char ** data1 = 0;
						data1 = parse_stdin_tolimit(spl->input_files[i][j]);
						if (data1)
							ft_putendl_fd("got data", STDERR_FILENO);
						else
							ft_putendl_fd("couldnt get data", STDERR_FILENO);
						//strs_to_fd(data1, STDOUT_FILENO);
						strs_to_fd(data1, fd);
					}
					close(fd);
					exit_status(0, data);
				}
				else
					waitpid(pid, &status, 0);
			}
			if (fd == -1)
				success = -1;
			if (!spl->input_files[i][j + 1] && fd != -1 && ft_putstr_fd("redirecting stdin", STDERR_FILENO))
				dup2(fd, STDIN_FILENO);
			close(fd);
		}
		//ft_putstr_fd(ft_strjoin(ft_itoa(i)," finished ins"), log_fd);
		if (success != -1)// && ft_putstr_fd("- redired in\n", log_fd))
			success = 1;
	}
	//else
	//	ft_putstr_fd("no input files", log_fd);
	j = -1;
	//redir output
	if (spl->output_files && spl->output_files[i][0])
	{
		while (spl->output_files[i][++j])
		{
			printf("output file = %s in mode %d\n", spl->output_files[i][j], spl->output_types[i][j]);
			if (!spl->output_types[i][j])
				fd = open(spl->output_files[i][j], O_RDWR | O_TRUNC | O_CREAT);
			else if (spl->output_types[i][j] == 1)
				fd = open(spl->output_files[i][j], O_APPEND | O_RDWR | O_CREAT);
			if (fd == -1)
				success = -1;
			if (!spl->output_files[i][j + 1] && fd != -1)
				dup2(fd, STDOUT_FILENO);
			if (fd >= 0)
				close(fd);
		}
		//ft_putstr_fd(ft_strjoin(ft_itoa(i)," finished outs"), log_fd);
		if (success == 1)// && ft_putstr_fd("- redired in and out\n", log_fd))
			success = 3;
		else if (!success)// && ft_putstr_fd("- redired out\n", log_fd))
			success = 2;
		else if (fd == -1)
			success = -1;
	}
	//else
	//	ft_putstr_fd("no output files", log_fd);
	//1 ON REDIRS STDIN 2 REDIRS STDOUT 3 REDIRS BOTH 0 ON NO REDIRS -1 ON ERROR
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
		redirs_status = handle_redirections(*i, data, pip);
		ft_putstr_fd("redirs status in ((i) = (status)) =", STDERR_FILENO);
		ft_putstr_fd(ft_strjoin(ft_itoa(*i), " ="), STDERR_FILENO);
		ft_putendl_fd(ft_itoa(redirs_status), STDERR_FILENO);
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
	temp_pids = (int *)ft_calloc(k + 2, sizeof(int));
	i = -1;
	while (++i <= k)
		temp_pids[i] = (*pids)[i];
	free(*pids);
	return (temp_pids);
}

static void free_spl_redirs(t_spl *cspl)
{
	int	i;
	int	j;

	i = -1;
	while (cspl->input_files && cspl->input_files[++i])
	{
		j = -1;
		while (cspl->input_files[i][++j])
			free(cspl->input_files[i][j]);
		free(cspl->input_files[i]);
		free(cspl->input_types[i]);
	}
	free(cspl->input_files);
	cspl->input_files = 0;
	free(cspl->input_types);
	i = -1;
	while (cspl->output_files && cspl->output_files[++i])
	{
		j = -1;
		while (cspl->output_files[i][++j])
			free(cspl->output_files[i][j]);
		free(cspl->output_files[i]);
		free(cspl->output_types[i]);
	}
	free(cspl->output_files);
	cspl->output_files = 0;
	free(cspl->output_types);
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
	free_spl_redirs(cspl);
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
	data->rl_addr = 0;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp, 0, 0);
	data->path = find_shell_path(g_envs);
	read_line = readline("shell:> ");
	while (read_line)
	{
		add_history(read_line);
		data->rl_addr = &read_line;
		spl = get_cmdsplit(read_line);
		fetch_cmdsplit(spl);
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
		j = 0;
		while(pids[j])
			j++;
		while (j > 0)
			waitpid(pids[--j], &data->ret, 0);
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
	exit_status(0, data);
}
