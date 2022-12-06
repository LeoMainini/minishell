/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_ac_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:30:42 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/06 15:05:17 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	iterarg(t_spl *spl, char *s, int *i, int *argc)
{
	char	q;

	if (s[(*i)] && s[(*i)] != '|')
	{
		if (!spl->quotebool)
			(*argc)++;
		spl->quotebool = 1;
		if (s[(*i)] == 34 || s[(*i)] == 39)
		{
			q = s[(*i)++];
			while (s[(*i)] && s[(*i)] != q)
				(*i)++;
			if (s[(*i)] && s[(*i)] == q)
			{
				(*i)++;
				spl->quotebool = 1;
				return (1);
			}
		}
		if (s[(*i)] && s[(*i)] != '|' && !ispc(s[(*i)]))
			(*i)++;
	}
	return (0);
}

int	skipspc_checkpipe(t_spl *spl, char *s, int *i)
{
	while (s[(*i)] && ispc(s[(*i)]))
	{
		spl->quotebool = 0;
		(*i)++;
	}
	if (!s[(*i)])
		return (1);
	if (s[(*i)] && s[(*i)] == '|')
	{
		(*i)++;
		return (1);
	}
	return (0);
}

int	arg_count(t_spl *spl, char *s, int l)
{
	static int	i;
	int			argc;

	spl->quotebool = 0;
	argc = 0;
	if (!l)
		i = 0;
	while (s[i])
	{
		if (skipspc_checkpipe(spl, s, &i))
			break ;
		if (iterarg(spl, s, &i, &argc))
			continue ;
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
		else if (s[i] && !ispc(s[i]) && s[i] != '|')
			i++;
		else if (s[i] && (ispc(s[i]) || s[i] == '|'))
			break ;
	}
	return (i - j);
}
