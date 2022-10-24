/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/24 16:06:35 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_status(int status, char **line)
{
	free(*line);
	rl_clear_history();
	exit(status);
}

int	main(void)
{
	char *read_line;

	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	read_line = readline("shell:>");
	while (read_line)
	{
		add_history(read_line);
		free(read_line);
		read_line = NULL;
		read_line = readline("shell:>");
	}
	exit_status(1, &read_line);
}
