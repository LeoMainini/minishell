/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/24 19:34:15 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **g_envs;

void exit_status(int status, char **line)
{
	free(*line);
	rl_clear_history();
	exit(status);
}

int	main(int agrc, char **argv, char **envp)
{
	char *read_line;

	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	read_line = readline("shell:>");
	while (read_line)
	{
		add_history(read_line);

		free(read_line);
		read_line = readline("shell:>");
	}
	exit_status(1, &read_line);
}
