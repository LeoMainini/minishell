/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:37:49 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/09 21:06:45 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"
#include	<unistd.h>

extern	char**g_envs;

char	*ft_strfree_join(char **s1, char *s2)
{
	char	*result;

	result = ft_strjoin(*s1, s2);
	free(*s1);
	return (result);
}

int	check_folder(t_cmdd *argd, int i)
{
	char *temp;
	if (!ft_strcmp(argd->args[1], "-"))
		return (3);
	if (access(argd->args[i], F_OK) && printf("File not found\n"))
		return (0);
	temp = ft_strjoin(argd->args[i], "/.");
	if ((access(argd->args[i], X_OK) || access(temp, F_OK) )
		&& printf("File is not a folder\n"))
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
		if (!ft_strcmp(file_path[k], ".."))
			steps_back++;
	k = -1;
	while (file_path[++k])
		free(file_path[k]);
	free(file_path);
	return (steps_back);
}

char	*join_chunks(char **str_chunks, char *sep, int limiter)
{
	int		k;
	char	*complete_str;

	k = -1;
	complete_str = 0;
	while (str_chunks[++k] && (k < limiter || limiter < 0))
	{
		if (!complete_str)
			complete_str = ft_strjoin(sep, str_chunks[k]);
		else
		{
			complete_str = ft_strfree_join(&complete_str, sep);
			complete_str = ft_strfree_join(&complete_str, str_chunks[k]);
		}
	}
	if (complete_str)
		return (ft_strfree_join(&complete_str, sep));
	return (complete_str);
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

int	change_back(t_ms *data, char *pwd, int fd, int before_pipe)
{
		t_cmdd	temp;
		int		i;
		
		ft_putendl_fd(*(get_env("OLDPWD", data)) + 7, fd);
		if (before_pipe)
			return (1);
		temp.args = ft_calloc(3, sizeof(char *));
		if(!temp.args)
			return (0);
		temp.args[0] = ft_strdup("export");
		temp.args[1] = ft_strjoin("OLDPWD=", pwd);
		chdir(*(get_env("OLDPWD", data)) + 7);
		i = export(&temp, data, 0);
		free (temp.args[1]);
		free (temp.args[0]);
		free (temp.args);
		return (i == 0);
}

int change_dir(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int		i;
	char	*absolute_path;
	char	*pwd;

	absolute_path = 0;
	if (!argd->args[1])
	{
		if (!before_pipe)
		{
			chdir(*(get_env("HOME", data)) + 5);
			if (!set_pwd(data))
				return (set_ret_return(data, 1));
		}
		return (set_ret_return(data, 0));
	}
	i = check_folder(argd, 1);
	if (!i)
		return (set_ret_return(data, 1));
	pwd = get_pwd();
	if (i == 1 && argd->args[1] && !before_pipe)
		chdir(argd->args[1]);
	if (i == 2 && argd->args[1] && !before_pipe)
	{
		absolute_path = rel_to_abs_pwd(argd, 1, pwd);
		chdir(absolute_path);
		free(absolute_path);
	}
	if (i == 3 && argd->args[1])
		if (!change_back(data, pwd, argd->out_fd,before_pipe)
			&& printf("Failed to change directory\n"))
			return (set_ret_return(data, 1));
	free(pwd);
	if (!set_pwd(data))
		return (set_ret_return(data, 1));
	return (set_ret_return(data, 0));
}