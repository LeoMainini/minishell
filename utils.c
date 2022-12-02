/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 17:26:35 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/02 16:40:43 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

extern char	**g_envs;

int	set_ret_return(t_ms *data, int code)
{
	data->ret = code;
	return (code);
}

void	cleanup_exec_data(t_ms *data, t_spl *spl, char **read_line)
{
	close(data->pip[0]);
	free_cmdsplit(spl);
	free(*read_line);
}

void	exit_status(int status, t_ms *data, unsigned int print_exit)
{
	free_data(data);
	if (print_exit)
		ft_putendl_fd("exit", STDERR_FILENO);
	exit(status);
}
