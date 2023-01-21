/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 15:34:10 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/21 17:22:38 by leferrei         ###   ########.fr       */
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
	if ((scmp(str, "9223372036854775807") > 0 && ft_strlen(str) >= 19)
		|| (scmp(str, "-9223372036854775808") > 0 && ft_strlen(str) >= 20))
		return (0);
	return (1);
}


void	free_data_exit_status(int status, t_ms *data, int print_exit)
{
	free_cmdsplit(get_cmdsplit(0), data);
	if (!data->pids_written)
		check_free_zeroout((void **)&data->pids);
	exit_status(status, data, print_exit);
}

int	check_alpha(t_cmdd *argd, int before_pipe, t_ms *data, int i)
{
	if (check_numeric(argd->args[i]))
		return (0);
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(argd->args[1], STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	if (!before_pipe)
		free_data_exit_status(2, data, 0);
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
			free_data_exit_status(0, data, 1);
	i = 0;
	argd->args[1] = ft_strtrim(argd->args[1], " \t\v\f\r\n");
	if (argd->args[2] && ft_putstr_fd("Too many arguments\n", STDERR_FILENO))
		if (!before_pipe && check_numeric(argd->args[1]))
			return (set_ret_return(data, 1));
	while (argd->args[++i])
		if (!check_alpha(argd, before_pipe, data, i))
			continue ;
	status = ft_atoi(argd->args[1]);
	if (status)
		code = 256 + status;
	if (!before_pipe)
		free_data_exit_status((int)code, data, 1);
	return (0);
}
