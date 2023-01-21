/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:59:48 by bcarreir          #+#    #+#             */
/*   Updated: 2023/01/21 15:57:50 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

void	hd_sighandler(int signum)
{
	char	*fp;

	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 0);
		rl_replace_line("", 1);
		fp = save_get_fp(0);
		check_free_zeroout((void **)&fp);
		exit(20);
		return ;
	}
}

void	cmd_sighandler(int signum)
{
	char	*fp;

	if (signum == SIGINT)
	{
		rl_replace_line("", 1);
		ft_putstr_fd("\n", 0);
		fp = save_get_fp(0);
		check_free_zeroout((void **)&fp);
		exit(130);
		return ;
	}
	if (signum == SIGQUIT)
	{
		rl_replace_line("", 1);
		ft_putstr_fd("\n", 0);
		fp = save_get_fp(0);
		check_free_zeroout((void **)&fp);
		exit(131);
		return ;
	}
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 1);
		ft_putstr_fd("\n", 0);
		rl_on_new_line();
		rl_redisplay();
		get_struct(0)->ret = 130;
		return ;
	}
	return ;
}
