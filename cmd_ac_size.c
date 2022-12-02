/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ac_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:30:42 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/02 16:37:09 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_count(t_spl *spl, char *s, int l)
{
	static int	i;
	char		q;
	int			argc;

	spl->quotebool = 0;
	argc = 0;
	if (!l)
		i = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
		{
			spl->quotebool = 0;
			i++;
		}
		if (!s[i])
			break ;
		if (s[i] && s[i] == '|')
		{
			i++;
			break ;
		}
		if (s[i] && s[i] != '|')
		{
			if (!spl->quotebool)
				argc++;
			spl->quotebool = 1;
			if (s[i] == 34 || s[i] == 39)
			{
				q = s[i++];
				while (s[i] && s[i] != q)
					i++;
				if (s[i] && s[i] == q)
				{
					i++;
					spl->quotebool = 1;
					continue ;
				}
			}
			if (s[i] && s[i] != '|' && !ft_isspace(s[i]))
				i++;
		}
	}
	return (argc);
}

int	ft_argsize(char *s, int i)
{
	char	q;
	int		j;

	j = i;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (s[i] && s[i] == q)
			{
				i++;
				continue ;
			}
		}
		else if (s[i] && !ft_isspace(s[i]) && s[i] != '|')
			i++;
		else if (s[i] && (ft_isspace(s[i]) || s[i] == '|'))
			break ;
	}
	return (i - j);
}
