/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/30 13:51:22 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

char **g_envs;

void exit_status(int status, char **line)
{
	free(*line);
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

int	main(int argc, char **argv, char **envp)
{
	char	*read_line;
	int		i;
	t_ms	data;

	(void)argc;
	(void)argv;
	data.ret = 0;
	if (data.ret)
		return (1);
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp, 0);
	read_line = readline("shell:> ");
	while (read_line)
	{
		add_history(read_line);
		cmd_split(read_line);
		char **temp = ft_split(read_line, ' ');
		if (temp && *temp)
		{
			t_cmdd cmds;
			cmds.args = temp;
			cmds.in_fd = 0;
			cmds.out_fd = 0;
			if (!ft_strcmp(temp[0], "cd"))
				change_dir(&cmds, &data);
			if (!ft_strcmp(temp[0], "env"))
				env(&cmds, &data);
			if (!ft_strcmp(temp[0], "pwd"))
				pwd(&cmds, &data);
			if (!ft_strcmp(temp[0], "export"))
				export(&cmds, &data);
			if (!ft_strcmp(temp[0], "unset"))
				unset(&cmds, &data);
		}
			free(read_line);
			read_line = readline("shell:> ");
	}
	i = -1;
	while (g_envs[++i])
		free(g_envs[i]);
	free(g_envs);
	exit_status(1, &read_line);
}
