/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 15:54:22 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/18 16:34:59 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>

int	pipex(int argc, char **argv, char **envp, t_ms *data2)
{
	char	*path;
	int		i;
	t_vars	*data;
	int		result;

	data = (t_vars *) malloc(sizeof(t_vars));
	if ((!data || !init_struct(data, argc, argv)) && printf("INIT ERROR \n"))
		return (1);
	i = -1;
	path = 0;
	path = find_shell_path(envp);
	if (!path && printf("Error finding shell path\n"))
		free_and_exit(data, 0, 0);
	while (data->cmds[++i])
	{
		if (!get_path(data, i, path, envp))
			break ;
		if (!data->cmds[1])
			exec_one(data, i, envp);
		else
			fork_lpipes_execute(data, i, envp, data2);
		if (data->path)
			free(data->path);
		data->path = 0;
	}
	while (--i > 0)
		wait(NULL);
	result = data->status;
	free_and_exit(data, 0, 0);
	return (result);
}
