/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:37:49 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 17:50:19 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"minishell.h"
#include	<unistd.h>

extern char	**g_envs;

int	change_back(t_ms *data, char *pwd, int fd, int before_pipe)
{
	t_cmdd	temp;
	int		i;

	ft_putendl_fd(*(get_env("OLDPWD", data)) + 7, fd);
	if (before_pipe)
		return (1);
	temp.args = ft_calloc(3, sizeof(char *));
	if (!temp.args)
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

int	exec_dir_change(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int		result;
	char	*pwd;
	char	*absolute_path;
	int		i;

	result = 0;
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
		check_free_zeroout((void **)&absolute_path);
	}
	if (i == 3 && argd->args[1])
		if (!change_back(data, pwd, argd->out_fd, before_pipe)
			&& ft_putstr_fd("Failed to change directory\n", STDERR_FILENO))
			result = 1;
	check_free_zeroout((void **)&pwd);
	return (result);
}

int	change_dir(t_cmdd *argd, t_ms *data, int before_pipe)
{
	int		result;

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
	result = exec_dir_change(argd, data, before_pipe);
	if (!set_pwd(data))
		return (set_ret_return(data, 1));
	return (set_ret_return(data, result));
}
