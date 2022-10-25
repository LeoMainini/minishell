/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:35:28 by leferrei          #+#    #+#             */
/*   Updated: 2022/10/25 16:59:59 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

char	*get_pwd()
{
	int		i;
	char	*resp;
	char	*pwd;

	i = 1;
	pwd = (char *)ft_calloc(i, sizeof(char));
	resp = getcwd(pwd, i);
	while (!resp && i++)
	{
		free(pwd);
		pwd = (char *)ft_calloc(i, sizeof(char));
		resp = getcwd(pwd, i);
	}
	return (pwd);
}