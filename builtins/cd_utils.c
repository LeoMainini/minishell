/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:47:35 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 14:49:31 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**g_envs;

int	check_folder(t_cmdd *argd, int i)
{
	char	*temp;

	if (!scmp(argd->args[1], "-"))
		return (3);
	if (access(argd->args[i], F_OK)
		&& ft_putstr_fd("File not found\n", STDERR_FILENO))
		return (0);
	temp = ft_strjoin(argd->args[i], "/.");
	if ((access(argd->args[i], X_OK) || access(temp, F_OK))
		&& ft_putstr_fd("File is not a folder\n", STDERR_FILENO))
	{
		free(temp);
		return (0);
	}
	free(temp);
	if (!ft_strncmp(argd->args[i], "..", 2)
		|| !ft_strncmp(argd->args[i], "./", 2))
		return (2);
	return (1);
}

int	steps_back(t_cmdd *argd, int i)
{
	char	**file_path;
	int		k;
	int		steps_back;

	k = -1;
	steps_back = 0;
	file_path = ft_split(argd->args[i], '/');
	while (file_path[++k])
		if (!scmp(file_path[k], ".."))
			steps_back++;
	k = -1;
	while (file_path[++k])
		free(file_path[k]);
	free(file_path);
	return (steps_back);
}

char	*rel_to_abs_pwd(t_cmdd *argd, int i, char *pwd)
{
	int		sb;
	char	**pwd_chunks;
	char	*absolute;
	int		k;

	sb = steps_back(argd, i);
	pwd_chunks = ft_split(pwd, '/');
	k = 0;
	while (pwd_chunks[k])
		k++;
	k = k - sb;
	absolute = join_chunks(pwd_chunks, "/", k);
	k = -1;
	while (pwd_chunks[++k])
		free(pwd_chunks[k]);
	free(pwd_chunks);
	return (absolute);
}
