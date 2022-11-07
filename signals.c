/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:59:48 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/07 16:15:37 by leferrei         ###   ########.fr       */
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
		rl_on_new_line();
		printf("\n");
		rl_redisplay();
		return ;
	}
	else if (signum == SIGQUIT)
		exit(131);
	return ;
}
