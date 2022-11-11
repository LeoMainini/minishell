/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/11 09:02:08 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <fcntl.h>
#include "lib-pipex/pipex.h"

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

int execute_builtin(char ***cmd_argvs, int k, t_ms *data)
{
	int	i;
	t_cmdd		cmds;


	i = check_builtin(cmd_argvs[k][0]);
	if (!i)
		return (0);
	cmds.args = cmd_argvs[k];
	cmds.in_fd = 0;
	
	if (data->builtins_outfd > -1)
		close(data->builtins_outfd);
	data->builtins_outfd = -1;
	if (data->system_outfd > -1)
		close(data->system_outfd);
	data->system_outfd = -1;
	data->builtins_outfd = open("./.temp_binout", O_RDWR | O_CREAT | O_TRUNC, 0666);
	printf("outfd in builtin = %d\n", data->builtins_outfd);
	if (cmd_argvs[k + 1] != 0)
		cmds.out_fd = data->builtins_outfd;
	else
	
		cmds.out_fd = STDOUT_FILENO;
	if (!interpret_strings(&cmds, data))
		printf("String missing quotes\n");
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
	return (1);
}

void	execute_system_funcs(char ***cmd_argv, int *i, t_ms *data)
{
	int	j;
	int	k;
	t_simargs *sim_args;

	sim_args = malloc(sizeof(t_simargs));
	if (!sim_args)
		return ;
	j = *i;
	while (cmd_argv[j] && cmd_argv[j][0] && !check_builtin(cmd_argv[j][0]) )
	{
		printf("oy\n");
		j++;
	}
	sim_args->argc = (j - *i) + 3;
	printf("value = %p, argc = %d\n", sim_args, sim_args->argc);
	sim_args->argv = ft_calloc(sim_args->argc + 1, sizeof(char *));
	if (!sim_args->argv)
		return ;
	k = 0;
	sim_args->argv[k++] = ft_strdup("");
	sim_args->argv[k++] = ft_itoa(data->builtins_outfd);
	while (cmd_argv[*i] && *i < j)
		sim_args->argv[k++] = join_chunks(cmd_argv[(*i)++], " ", -1);
	if (data->system_outfd == -1)
		data->system_outfd = open("./.temp_sysout", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (sim_args->argc == 4)
		dup2(STDOUT_FILENO, data->system_outfd);
	sim_args->argv[k] = ft_itoa(data->system_outfd);
	printf("infd in pipex = %d out fd = %d\n", data->builtins_outfd, data->system_outfd);
	k = -1;
	while (sim_args->argv[++k])
		printf("argc %d = %s\n", k, sim_args->argv[k]);
	pipex(sim_args->argc, sim_args->argv, g_envs);
	close(data->system_outfd);
	data->system_outfd = -1;
	free(sim_args->argv);
	free(sim_args);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read_line;
	t_ms		data;
	char		***temp;
	char		*sys_output;
	int			i;

	(void)argc;
	(void)argv;
	data.ret = 0;
	data.builtins_outfd = -1;
	data.system_outfd = -1;
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp, 0);
	read_line = readline("shell:> ");
	while (read_line)
	{
		add_history(read_line);
		data.rl_addr = &read_line;
		temp = cmd_split(read_line);
		(void)temp;
		(void)i;
		(void)data;

		if (temp && *temp && **temp)
		{
			i = -1;
			while (temp[++i])
			{
				if (!execute_builtin(temp, i, &data) && ft_strlen(temp[i][0]) > 1)
					execute_system_funcs(temp, &i, &data);
				if (!temp[i])
					break ;
			}
		}
		data.system_outfd = open(".temp_sysout", O_RDONLY);
		sys_output = get_next_line(data.system_outfd);
		if (sys_output)
			printf("here\n");
		printf("here\n");
		while(sys_output)
		{
			printf(sys_output);
			free(sys_output);
			sys_output = get_next_line(data.system_outfd);
		}
		close(data.system_outfd);
		data.system_outfd = -1;
		free(read_line);
		read_line = readline("shell:> ");
	}
	exit_status(1, &read_line);
}
