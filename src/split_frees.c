/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_frees.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 02:19:57 by benmonico         #+#    #+#             */
/*   Updated: 2023/01/24 16:28:51 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	alloc_redir_arrays(t_spl *spl)
{
	spl->output_files = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	spl->input_files = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	spl->output_types = ft_calloc(sizeof(int *), (spl->cmd_count + 1));
	spl->input_types = ft_calloc(sizeof(int *), (spl->cmd_count + 1));
	if (!spl->input_types || !spl->output_types || !spl->input_files
		|| !spl->output_files)
	{
		check_free_zeroout((void **)spl->input_types);
		check_free_zeroout((void **)spl->output_types);
		check_free_zeroout((void **)spl->input_files);
		check_free_zeroout((void **)spl->output_files);
	}
}

int	check_free_spl_strings(t_spl *spl)
{
	if (!spl->ss || !spl->str)
	{		
		check_free_zeroout((void **)spl->ss);
		check_free_zeroout((void **)spl->str);
		return (1);
	}
	return (0);
}
