/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:34:10 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/09 16:31:30 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	i--;
	while (str[++i])
		if (str[i] < '0' || str[i] > '9')
			return (0);
	return (1);
}

int	exit_shell(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int				status;
	unsigned char	code;

	code = 0;
	if (!argd->args[1])
		if (!before_pipe)
			exit_status(0, data->rl_addr);
	if (argd->args[2] && printf("Too many arguments\n"))
		if (!before_pipe)
			return (set_ret_return(data, 1));
	if (!check_numeric(argd->args[1]))
	{
		printf("exit: %s: numeric argument required\n", argd->args[1]);
		if (!before_pipe)
			exit_status(2, data->rl_addr);
	}
	status = ft_atoi(argd->args[1]);
	if (status)
		code = 256 + status;
	if (!before_pipe)
		exit_status((int)code, data->rl_addr);
	return (0);
}