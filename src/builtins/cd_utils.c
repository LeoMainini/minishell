/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:47:35 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/23 16:56:22 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

int	check_folder(t_cmdd *argd, int i)
{
	char	*temp;

	if (!scmp(argd->args[1], "-"))
		return (3);
	if (access(argd->args[i], F_OK)
		&& ft_putstr_fd("File or folder not found\n", STDERR_FILENO))
		return (0);
	temp = ft_strjoin(argd->args[i], "/.");
	if ((access(argd->args[i], X_OK) || access(temp, F_OK))
		&& ft_putstr_fd("File is not a folder\n", STDERR_FILENO))
	{
		check_free_zeroout((void **)&temp);
		return (0);
	}
	check_free_zeroout((void **)&temp);
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
	while (file_path && file_path[++k])
		if (!scmp(file_path[k], ".."))
			steps_back++;
	k = -1;
	while (file_path && file_path[++k])
		check_free_zeroout((void **)&file_path[k]);
	check_free_zeroout((void **)&file_path);
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
	if (!pwd_chunks)
		return (0);
	k = 0;
	while (pwd_chunks && pwd_chunks[k])
		k++;
	k = k - sb;
	absolute = join_chunks(pwd_chunks, "/", k);
	if (!absolute)
		return (0);
	k = -1;
	while (pwd_chunks && pwd_chunks[++k])
		check_free_zeroout((void **)&pwd_chunks[k]);
	check_free_zeroout((void **)&pwd_chunks);
	return (absolute);
}
