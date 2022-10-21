/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m-shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 16:23:29 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/21 16:23:31 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void exit_status(int status)
{
	rl_clear_history();
	exit(status);
}

int	main(void)
{
	char *read_line;

	read_line = readline("shell:>");
	while (read_line)
	{
		add_history(read_line);
		free(read_line);
		read_line = readline("shell:>");
	}
	exit_status(1);
}
