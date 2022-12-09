/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:28 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 17:50:19 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*get_pwd(void)
{
	int		i;
	char	*resp;
	char	*pwd;

	i = 1;
	pwd = (char *)ft_calloc(i, sizeof(char));
	resp = getcwd(pwd, i);
	while (!resp && i++)
	{
		check_free_zeroout((void **)&pwd);
		pwd = (char *)ft_calloc(i, sizeof(char));
		resp = getcwd(pwd, i);
	}
	return (pwd);
}

int	pwd(t_cmdd *argd, t_ms *data)
{
	char	*pwd;

	if (argd->args[1] && ft_putstr_fd("Too many arguments.\n", STDERR_FILENO))
		return (set_ret_return(data, 1));
	pwd = get_pwd();
	if (!pwd)
		return (set_ret_return(data, 1));
	ft_putendl_fd(pwd, argd->out_fd);
	check_free_zeroout((void **)&pwd);
	return (set_ret_return(data, 0));
}

int	set_pwd(t_ms *data)
{
	char	*pwd;
	t_cmdd	temp;
	int		i;

	pwd = get_pwd();
	if (!pwd)
		return (0);
	temp.args = ft_calloc(3, sizeof(char *));
	if (!temp.args)
		return (0);
	temp.args[0] = ft_strdup("export");
	temp.args[1] = ft_strjoin("PWD=", pwd);
	i = export(&temp, data, 0);
	check_free_zeroout((void **)&pwd);
	free (temp.args[1]);
	free (temp.args[0]);
	free (temp.args);
	return (i == 0);
}
