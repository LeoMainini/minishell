/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 13:20:54 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 13:30:25 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**g_envs;

char	*get_var_name(t_cmdd *argd, int i)
{
	if (ft_strchr(argd->args[i], '=') && scmp(argd->args[i], "="))
		return (ft_substr(argd->args[i], 0,
			(ft_strchr(argd->args[i], '=') - argd->args[i])));
	else 
		return (ft_strdup(argd->args[i]));
}

int	name_invalid(char *name)
{
	int	result;

	if (!is_alphastr(name))
	{
		ft_putstr_fd("export: '", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	export(t_cmdd *argd, t_ms *data, int before_pipe)
{
	char	**envi;
	int		last_i;
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
		if (name_invalid(name) || before_pipe)
			continue ;
		envi = get_env(name, data);
		if (envi)
		{
			if (ft_strchr(argd->args[i], '='))
			{
				free(*envi);
				*envi = ft_strdup(argd->args[i]);
			}
			free(name);
		}
		else
		{
			free(name);
			envi = g_envs;
			g_envs = duplicate_envp(g_envs, 1, 1);
			free(envi);
			last_i = 0;
			while (g_envs[last_i])
				last_i++;
			g_envs[last_i] = ft_strdup(argd->args[i]);
		}
	}
	return (set_ret_return(data, result));
}