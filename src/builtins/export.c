/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:20:54 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/09 18:30:14 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**g_envs;

char	*get_var_name(t_cmdd *argd, int i)
{
	if (ft_strchr(argd->args[i], '=') && scmp(argd->args[i], "="))
		return (ft_substr(argd->args[i], 0,
				(ft_strchr(argd->args[i], '=') - argd->args[i])));
	else
		return (ft_strdup(argd->args[i]));
}

int	name_invalid(char *name, int print)
{
	if (!is_alphastr(name))
	{
		if (!print)
			return (1);
		ft_putstr_fd("export: '", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	return (0);
}

void	add_genv_value(char **envi, t_cmdd *argd, int i, char *name)
{
	int	last_i;

	if (envi)
	{
		if (ft_strchr(argd->args[i], '='))
		{
			check_free_zeroout((void **)&*envi);
			*envi = ft_strdup(argd->args[i]);
		}
		check_free_zeroout((void **)&name);
	}
	else
	{
		check_free_zeroout((void **)&name);
		envi = g_envs;
		g_envs = duplicate_envp(g_envs, 1, 1);
		check_free_zeroout((void **)&envi);
		last_i = 0;
		while (g_envs[last_i])
			last_i++;
		g_envs[last_i] = ft_strdup(argd->args[i]);
	}
}

int	export(t_cmdd *argd, t_ms *data, int before_pipe)
{
	char	**envi;
	char	*name;
	int		i;
	int		result;

	envi = 0;
	result = 0;
	if (!argd->args[1])
		return (print_sorted_envs(argd->out_fd));
	i = 0;
	while (argd->args[++i])
	{
		name = get_var_name(argd, i);
		if (!result)
			result = name_invalid(name, 1);
		if (name_invalid(name, 0) || before_pipe)
		{
			check_free_zeroout((void **)&name);
			continue ;
		}
		envi = get_env(name, data);
		add_genv_value(envi, argd, i, name);
	}
	return (set_ret_return(data, result));
}
