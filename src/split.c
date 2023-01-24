/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2023/01/24 16:28:15 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_redirs(t_spl *spl)
{
	if (validate_redirs(spl))
		return ;
	if (spl->redir_bool)
	{
		alloc_redir_arrays(spl);
		init_redir_arrays(spl);
		dupwithoutredirs(spl);
	}
}

int	verify_alloc_ss(char *s, t_spl *spl)
{
	int		l;
	int		ac;

	init_spl(spl);
	if (!s)
		return (1);
	if (isvalidcmd(s, spl))
		return (1);
	spl->str = separate_redirs(s);
	if (!spl->str)
		spl->str = ft_strdup(s);
	spl->ss = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	if (check_free_spl_strings(spl))
		return (1);
	l = -1;
	while (++l < spl->cmd_count)
	{
		ac = arg_count(spl, spl->str, l);
		if (ac == 0)
			return (1);
		spl->ss[l] = ft_calloc(sizeof(char *), (ac + 1));
		if (!spl->ss[l])
			return (1);
	}
	return (0);
}

int	get_new_arg(t_spl *spl, int *i, int *l, int *j)
{
	int	k;

	k = 0;
	while (spl->str[(*i)] && ispc(spl->str[(*i)]))
		(*i)++;
	if (!spl->str[(*i)])
		return (1);
	if (spl->str[(*i)] && spl->str[(*i)] == '|')
	{
		(*l)++;
		(*j) = 0;
		(*i)++;
		while (spl->str[(*i)] && ispc(spl->str[(*i)]))
			(*i)++;
	}
	if (spl->str[(*i)] && spl->str[(*i)] != '|')
	{
		k = ft_argsize(spl->str, (*i));
		spl->ss[(*l)][(*j)] = ft_calloc(sizeof(char), (k + 1));
		if (!spl->ss[(*l)][(*j)])
			return (1);
	}
	return (0);
}

void	copytoarg(t_spl *spl, int *i, int*j, int *l)
{
	int		k;
	int		q;

	k = 0;
	while (spl->str[(*i)])
	{
		while (spl->str[(*i)] && !ispc(spl->str[(*i)]) && spl->str[(*i)] != '|')
		{
			if ((spl->str[(*i)] == 34 || spl->str[(*i)] == 39))
			{
				q = spl->str[(*i)];
				spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
				while (spl->str[(*i)] && spl->str[(*i)] != q)
					spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
				if (spl->str[(*i)] && spl->str[(*i)] == q)
				{
					spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
					continue ;
				}
			}
			spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
		}
		if (spl->str[(*i)] && (ispc(spl->str[(*i)]) || spl->str[(*i)] == '|'))
			break ;
	}
}

t_spl	cmd_split(char *s)
{
	t_spl	spl;
	int		i;
	int		j;
	int		l;

	j = 0;
	l = 0;
	i = 0;
	if (verify_alloc_ss(s, &spl))
	{
		check_free_zeroout((void **)&spl.str);
		free_cmdarray(&spl);
		return (spl);
	}
	while (spl.str[i])
	{
		if (get_new_arg(&spl, &i, &l, &j))
			break ;
		copytoarg(&spl, &i, &j, &l);
		j++;
	}
	manage_redirs(&spl);
	check_free_zeroout((void **)&spl.str);
	spl.str = NULL;
	return (spl);
}
