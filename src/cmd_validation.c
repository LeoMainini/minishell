/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:07:13 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/06 15:39:27 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ffquotedtext(t_spl *spl, char *s, int *j, char qt)
{
	spl->quotebool *= -1;
	(*j)++;
	while (s[(*j)] && s[(*j)] != qt)
		(*j)++;
	if (s[(*j)] == qt)
	{
		spl->quotebool *= -1;
		(*j)++;
		return (0);
	}
	else
		return (1);
}

int	invalidpipe(char *s, t_spl *spl, int *j, int *k)
{
	if (s[(*j)] == '|')
	{
		(*k) = (*j) + 1;
		while (s[(*k)] && ispc(s[(*k)]))
			(*k)++;
		if (!s[(*k)] || s[(*k)] == '|' || s[(*j) + 1] == '|' || (*j) == 0)
		{
			printf("parse error near `|'\n");
			return (1);
		}
		spl->cmd_count++;
	}
	return (0);
}

int	isvalidcmd(char *s, t_spl *spl)
{
	int	j;
	int	k;

	if (*s)
		spl->cmd_count++;
	j = 0;
	while (s[j])
	{
		if (s[j] == 34 || s[j] == 39)
		{
			if (ffquotedtext(spl, s, &j, s[j]))
			{
				printf("parse error missing quotes\n");
				return (1);
			}
			continue ;
		}
		if (invalidpipe(s, spl, &j, &k))
			return (1);
		j++;
	}
	return (0);
}

void	init_spl(t_spl *spl)
{
	spl->ss = NULL;
	spl->str = NULL;
	spl->input_files = NULL;
	spl->output_files = NULL;
	spl->cmd_count = 0;
	spl->quotebool = 1;
	spl->redir_bool = 0;
	spl->input_types = 0;
	spl->output_types = 0;
	spl->redir_in = 0;
	spl->redir_out = 0;
}
