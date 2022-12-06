/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:38:25 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 14:50:00 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_cmdd *argd, t_ms *data)
{
	int	n_line;
	int	i;
	int	printed_c;

	n_line = 1;
	if (argd->args[1] && !scmp(argd->args[1], "-n"))
		n_line = 0;
	i = 1;
	while (argd->args[1] && argd->args[++i - n_line])
	{
		printed_c = ft_putstr_fd(argd->args[i - n_line], argd->out_fd);
		if (argd->args[i - n_line + 1] && printed_c)
			ft_putstr_fd(" ", argd->out_fd);
	}
	if (n_line)
		ft_putstr_fd("\n", argd->out_fd);
	return (set_ret_return(data, 0));
}
