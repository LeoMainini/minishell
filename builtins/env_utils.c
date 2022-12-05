/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 14:29:49 by leferrei          #+#    #+#             */
/*   Updated: 2022/12/05 14:56:08 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**g_envs;

char	**get_sep_env_values(char *str)
{
	char	**index;

	if (ft_strchr(str, '='))
	{
		index = ft_calloc(3, sizeof(char *));
		index[0] = ft_substr(str, 0, (ft_strchr(str, '=') - str));
		index[1] = ft_substr(str, (ft_strchr(str, '=') - str) + 1,
				ft_strlen(str) - (ft_strchr(str, '=') - str));
		index[2] = 0;
	}
	else
	{
		index = ft_calloc(2, sizeof(char *));
		index[0] = ft_strdup(str);
		index[1] = 0;
	}
	return (index);
}

int	get_index(char	*env)
{
	int		i;
	int		k;
	int		result;
	char	**sep_env;

	i = -1;
	result = -1;
	while (g_envs[++i])
	{
		k = -1;
		sep_env = get_sep_env_values(g_envs[i]);
		if (!scmp((const char *)env, (const char *)(sep_env[0])))
		{
			while (sep_env[++k])
				free(sep_env[k]);
			free(sep_env);
			result = i;
			break ;
		}
		while (sep_env[++k])
			free(sep_env[k]);
		free(sep_env);
	}
	return (result);
}

char	**get_env(char *env, t_ms *data)
{
	int	i;

	(void)data;
	i = -1;
	i = get_index(env);
	if (i == -1)
		return (0);
	return (&g_envs[i]);
}

void	ft_str_swap(char ***s1, char ***s2)
{
	char	**temp;

	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void	print_free_3darray(char ****str_array, int fd)
{
	int		i;

	i = -1;
	while ((*str_array)[++i])
	{
		if ((*str_array)[i][1])
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd((*str_array)[i][0], fd);
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd((*str_array)[i][1], fd);
			ft_putstr_fd("\"\n", fd);
			free((*str_array)[i][0]);
			free((*str_array)[i][1]);
		}
		else
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd((*str_array)[i][0], fd);
			free((*str_array)[i][0]);
		}
		free((*str_array)[i]);
	}
	free((*str_array));
}
