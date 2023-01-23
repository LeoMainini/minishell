/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 17:26:35 by leferrei          #+#    #+#             */
/*   Updated: 2023/01/23 16:35:46 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

extern char	**g_envs;

int	set_ret_return(t_ms *data, int code)
{
	data->ret = code;
	return (code);
}

void	free_strarray(char **stra)
{
	int	i;

	i = -1;
	while (stra[++i])
		check_free_zeroout((void **)&stra[i]);
	free(stra);
}

void	exit_status(int status, t_ms *data, unsigned int print_exit)
{
	free_data(data);
	if (print_exit)
		ft_putendl_fd("exit", STDERR_FILENO);
	exit(status);
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

void	check_free_zeroout(void **ptr)
{
	if (!*ptr)
		return ;
	free(*ptr);
	*ptr = 0;
}
