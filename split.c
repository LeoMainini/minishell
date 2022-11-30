/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/30 17:31:01 by bcarreir         ###   ########.fr       */
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
	if (!spl->ss)
		return (1);
	s = spl->str;
	l = 0;
	while (l < spl->cmd_count)
	{
		ac = arg_count(spl, s, l);
		if (ac == 0)
			return (1);
		spl->ss[l] = ft_calloc(sizeof(char *), (ac + 1));
		if (!spl->ss[l])
			return (1);
		l++;
	}
	return (0);
}



int	get_new_arg(t_spl *spl, int *i, int *l, int *j)
{
	int	k;

	k = 0;
	while (spl->str[(*i)] && ft_isspace(spl->str[(*i)]))
		(*i)++;
	if (!spl->str[(*i)])
		return (1);
	if (spl->str[(*i)] && spl->str[(*i)] == '|')
	{
		(*l)++;
		(*j) = 0;
		(*i)++;
		while (spl->str[(*i)] && ft_isspace(spl->str[(*i)]))
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
	char	*s;
	int		k;
	int		q;

	k = 0;
	s = spl->str;
	while (s[(*i)])
	{
		while (s[(*i)] && !ft_isspace(s[(*i)]) && s[(*i)] != '|')
		{
			if ((s[(*i)] == 34 || s[(*i)] == 39))
			{
				q = s[(*i)];
				spl->ss[(*l)][(*j)][k++] = s[(*i)++];
				while (s[(*i)] && s[(*i)] != q)
					spl->ss[(*l)][(*j)][k++] = s[(*i)++];
				if (s[(*i)] && s[(*i)] == q)
				{
					spl->ss[(*l)][(*j)][k++] = s[(*i)++];
					continue ;
				}
			}
			spl->ss[(*l)][(*j)][k++] = s[(*i)++];
		}
		if (s[(*i)] && (ft_isspace(s[(*i)]) || s[(*i)] == '|'))
			break ;
	}
	(*j)++;
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
		return (spl);
	while (spl.str[i])
	{
		if (get_new_arg(&spl, &i, &l, &j))
			break ;
		copytoarg(&spl, &i, &j, &l);
	}
	manage_redirs(&spl);
	free(spl.str);
	spl.str = NULL;
	return (spl);
}
