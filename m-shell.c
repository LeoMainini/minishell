/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/25 14:40:13 by leferrei         ###   ########.fr       */
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

char	**duplicate_envp(char **envs)
{
	int		i;
	char	**temp;

	if (!envs)
		return (0);
	i = 0;
	while (envs[i])
		i++;
	temp = (char **)ft_calloc(i + 1, sizeof(char *));
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

	(void)argc;
	(void)argv;

	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	g_envs = duplicate_envp(envp);
	if (check_envp_duplicate_error(envp))
		return (check_envp_duplicate_error(envp));
	printf("GOT VARS WOOO\n");
	read_line = readline("shell:> ");
	while (read_line)
	{
		add_history(read_line);

		free(read_line);
		read_line = readline("shell:> ");
	}
	exit_status(1, &read_line);
}
