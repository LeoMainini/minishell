/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:09:09 by leferrei          #+#    #+#             */
/*   Updated: 2022/07/27 15:09:10 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>

int	write_array_to_fd(char **array,	int fds[2], int *result)
{
	int	i;

	if (!array)
		return (-1);
	i = -1;
	while (array[++i])
		if (write(fds[1], array[i], ft_strlen(array[i])) == -1)
			return (-1);
	*result = dup2(fds[0], STDIN_FILENO);
	return (1);
}

int	dupe_pipes(t_vars *data, int i)
{
	int	result;

	if ((i == data->arg_count - 4 && !data->here_doc)
		|| (i == data->arg_count - 5 && data->here_doc))
		result = dup2(data->out_fd, STDOUT_FILENO);
	else if ((i && i < data->arg_count - 4 && !data->here_doc)
		|| (i && i < data->arg_count - 5 && data->here_doc))
		result = dup2(data->xfds[1], STDOUT_FILENO);
	if (i && result != -1)
		result = dup2(data->xfds[0], STDIN_FILENO);
	else if (result != -1)
	{
		result = dup2(data->fds[1], STDOUT_FILENO);
		if (!data->here_doc)
		{
			if (result != -1)
				result = dup2(data->in_fd, STDIN_FILENO);
			close(data->in_fd);
		}
		else
			write_array_to_fd(data->lines_in, data->hd_fds, &result);
	}
	return (result);
}

void	exec_child(t_vars *data, char **cmd_argv, int i, char **envp)
{
	if (dupe_pipes(data, i) == -1)
		free_and_exit(data, 5);
	close(data->hd_fds[0]);
	close(data->hd_fds[1]);
	close(data->fds[0]);
	close(data->fds[1]);
	close(data->xfds[0]);
	close(data->xfds[1]);
	if (execve(data->path, cmd_argv, envp) == -1)
		perror("Failed executing");
	ft_putstr_fd("Command not found: ", STDERR_FILENO);
	ft_putendl_fd(data->cmds[i][0], STDERR_FILENO);
	free_and_exit(data, 1);
}

void	exec_parent(t_vars *data, int i, int pid)
{
	int		status;

	if (!i)
		close(data->fds[1]);
	status = 0;
	close(data->hd_fds[0]);
	close(data->hd_fds[1]);
	close(data->xfds[0]);
	close(data->xfds[1]);
	if (i == 1 && !data->here_doc)
		close(data->in_fd);
	if ((i == data->arg_count - 4 && !data->here_doc)
		|| (i == data->arg_count - 5 && data->here_doc))
		waitpid(pid, &status, 0);
}

int	fork_lpipes_execute(t_vars *data, int i, char **envp)
{
	pid_t	pid;

	if (i)
	{
		data->xfds[0] = data->fds[0];
		if (pipe(data->fds) == -1)
			free_and_exit(data, 1);
		data->xfds[1] = data->fds[1];
	}
	pid = fork();
	if (pid == -1 && ft_printf("Fork Error\n"))
		return (1);
	else if (pid == 0)
		exec_child(data, data->cmds[i], i, envp);
	else
		exec_parent(data, i, pid);
	return (0);
}
