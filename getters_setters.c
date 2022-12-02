/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters_setters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 15:20:01 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/02 16:38:54 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include "lib-pipex/pipex.h"
#include <sys/wait.h>
#include <unistd.h>

extern char	**g_envs;

t_ms	*get_struct(t_ms **data)
{
	static t_ms	*result;

	if (data)
		result = *data;
	return (result);
}

t_spl	*set_cmdsplit(char *read_line)
{
	t_spl	*temp;

	temp = (t_spl *)ft_calloc(1, sizeof(t_spl));
	if (!temp)
		return (0);
	*temp = cmd_split(read_line);
	return (temp);
}

t_spl	*get_cmdsplit(t_spl *cmdsplit)
{
	static t_spl	*temp;

	if (cmdsplit)
		temp = cmdsplit;
	return (temp);
}
