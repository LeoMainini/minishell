/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:10:39 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/09 17:50:19 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmdarray(t_spl *spl)
{
	int	i;
	int	j;

	i = -1;
	while (spl->ss && spl->ss[++i])
	{
		j = -1;
		while (spl->ss[i][++j])
			check_free_zeroout((void **)&spl->ss[i][j]);
		check_free_zeroout((void **)&spl->ss[i]);
	}
	if (spl->ss)
		check_free_zeroout((void **)&spl->ss);
	spl->ss = NULL;
}

void	get_nbr_redirs_and_alloc(t_spl *spl, char ****aux)
{
	int	i;
	int	j;
	int	l;

	l = -1;
	while (spl->ss[++l])
	{
		i = 0;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<")
				|| !scmp(spl->ss[l][j], "<<")
					|| !scmp(spl->ss[l][j], ">")
						|| !scmp(spl->ss[l][j], ">>"))
				j++;
			else
				i++;
		}
		(*aux)[l] = ft_calloc(i + 1, sizeof(char *));
		if (!(*aux)[l])
			return ;
	}
}

void	duptoaux(t_spl *spl, char ****aux)
{
	int		l;
	int		j;
	int		i;

	l = -1;
	while (spl->ss[++l])
	{
		i = -1;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<")
				|| !scmp(spl->ss[l][j], "<<")
					|| !scmp(spl->ss[l][j], ">")
						|| !scmp(spl->ss[l][j], ">>"))
					j++;
			else
			{
				(*aux)[l][++i] = ft_strdup(spl->ss[l][j]);
				if (!(*aux)[l][i])
					return ;
			}
		}
	}
}

void	dupwithoutredirs(t_spl *spl)
{
	char	***aux;

	aux = ft_calloc(spl->cmd_count + 1, sizeof(char **));
	if (!aux)
		return ;
	get_nbr_redirs_and_alloc(spl, &aux);
	duptoaux(spl, &aux);
	free_cmdarray(spl);
	spl->ss = aux;
}
