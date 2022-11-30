/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:08:53 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/30 16:46:06 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line/get_next_line.h"

int	check_hd_get_cmds(t_vars *data, int argc, char **argv)
{
	if (!scmp(argv[1], "here_doc"))
	{
		data->lines_in = parse_stdin_tolimit(argv[2]);
		data->here_doc = 1;
		data->cmds = get_hd_commands(argc, argv);
		data->out_fd = ft_atoi(argv[argc - 1]);
		if (data->out_fd < 0 && printf("ERROR:\tOutput file error\n"))
			return (0);
	}
	else
	{
		data->here_doc = 0;
		data->lines_in = 0;
		data->cmds = get_commands(argc, argv);
		data->in_fd = ft_atoi(argv[1]);
		data->out_fd = ft_atoi(argv[argc - 1]);
		if (data->out_fd < 0 && printf("ERROR:\tOutput file error\n"))
			return (0);
	}
	return (1);
}

int	init_struct(t_vars *data, int argc, char **argv)
{
	data->cmds = 0;
	if (argc < 4 && printf("ERROR:\tMissing arguments\n"))
		return (0);
	data->arg_count = argc;
	if (!scmp(argv[1], argv[argc - 1])
		&& printf("ERROR:\tInfile same as outfile\n"))
		return (0);
	if (!check_hd_get_cmds(data, argc, argv) && printf("Init error\n"))
		return (0);
	if (!data->cmds && !data->lines_in
		&& printf("ERROR:\tArgument parsing error\n"))
		return (0);
	if (pipe(data->fds) == -1 && printf("Pipe Error\n"))
		return (0);
	if (pipe(data->xfds) == -1 && printf("Pipe Error\n"))
		return (0);
	if (pipe(data->hd_fds) == -1 && printf("Pipe Error\n"))
		return (0);
	return (1);
}

char	*get_p_pwd(char **envp)
{
	int		i;
	char	*pwd;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp(envp[i], "PWD", 3))
			pwd = ft_strdup(&envp[i][4]);
	if (pwd)
		return (ft_strfree_join(&pwd, "/"));
	return (0);
}

int	check_file_cmd(t_vars *data, int i)
{
	if (!strchr(data->cmds[i][0], '.') && !strchr(data->cmds[i][0], '/'))
		return (0);
	if (access(data->cmds[i][0], F_OK) && printf("File not found\n"))
		return (0);
	if (access(data->cmds[i][0], X_OK) && printf("File not executable\n"))
		return (0);
	if (!ft_strncmp(data->cmds[i][0], "..", 2)
		|| !ft_strncmp(data->cmds[i][0], "./", 2))
		return (2);
	return (1);
}

int	get_path(t_vars *data, int i, char *path, char **envp)
{
	char	*file;

	if (check_file_cmd(data, i) == 2)
	{
		data->path = get_p_pwd(envp);
		file = ft_strtrim(data->cmds[i][0], "./");
		data->path = absolute_to_relative_pwd(data, i, data->path);
		data->path = ft_strfree_join(&data->path, file);
		free(file);
	}
	else if (check_file_cmd(data, i))
		data->path = ft_strdup(data->cmds[i][0]);
	else
		data->path = ft_strjoin(path, data->cmds[i][0]);
	if (data->path)
		return (1);
	return (0);
}
