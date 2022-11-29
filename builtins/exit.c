/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:34:10 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/29 15:33:37 by leferrei         ###   ########.fr       */
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
	if ((ft_strcmp(str, "9223372036854775807") > 0 && ft_strlen(str) >= 19)
		|| (ft_strcmp(str, "-9223372036854775808") > 0 && ft_strlen(str) >= 20))
		return (0);
	return (1);
}

int	exit_shell(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int				status;
	unsigned char	code;
	int				i;

	code = 0;
	if (!argd->args[1])
		if (!before_pipe)
			exit_status(0, data);

	i = 0;
	while (argd->args[++i])
	{
		if (check_numeric(argd->args[i]))
			continue ;
		ft_putstr_fd("exit: ", STDERR_FILENO);
		ft_putstr_fd(argd->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);

		if (!before_pipe)
			exit_status(2, data);
	}
	if (argd->args[2] && ft_putstr_fd("Too many arguments\n", STDERR_FILENO))
		if (!before_pipe)
			return (set_ret_return(data, 1));
	status = ft_atoi(argd->args[1]);
	if (status)
		code = 256 + status;
	if (!before_pipe)
		exit_status((int)code, data);
	return (0);
}