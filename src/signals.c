/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:59:48 by bcarreir          #+#    #+#             */
/*   Updated: 2023/01/09 15:54:27 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		get_struct(0)->ret = 130;
		return ;
	}
	else if (signum == SIGQUIT)
		return ;
	return ;
}
