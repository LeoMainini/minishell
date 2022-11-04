/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:38:25 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/04 19:57:45 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_cmdd *argd, t_ms *data)
{
	int	n_line;
	int	i;
	int	printed_c;

	n_line = 1;
	if (argd->args[1] && !ft_strcmp(argd->args[1], "-n"))
		n_line = 0;
	i = 1;
	while (argd->args[++i - n_line])
	{
		printed_c = printf("%s", argd->args[i - n_line]);
		if (argd->args[i - n_line + 1] && printed_c)
			printf(" ");
	}
	if (n_line)
		printf("\n");
	return (set_ret_return(data, 0));
}