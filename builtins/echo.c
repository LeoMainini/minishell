/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 16:38:25 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/31 16:49:12 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(t_cmdd *argd, t_ms *data)
{
	int	n_line;
	int	i;

	n_line = 1;
	if (argd->args[1] && !ft_strcmp(argd->args[1], "-n"))
		n_line = 0;
	i = 1;
	while (argd->args[++i - n_line])
	{
		printf("%s", argd->args[i - n_line]);
		if (argd->args[i - n_line + 1])
			printf(" ");
	}
	if (n_line)
		printf("\n");
	return (set_ret_return(data, 0));
}