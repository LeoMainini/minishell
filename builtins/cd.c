/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:37:49 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/25 15:08:21 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"
#include	"../pipex/pipex.h"

int change_dir(char **path, t_cmdd *data, t_ms *data)
{
    if (!path)
        return (1);
	
}