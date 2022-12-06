/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirs2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:12:50 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/06 15:27:18 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	alloc_redir_arrays(t_spl *spl)
{
	spl->output_files = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	if (!spl->output_files)
		return ;
	spl->input_files = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	if (!spl->input_files)
		return ;
	spl->output_types = ft_calloc(sizeof(int *), (spl->cmd_count + 1));
	if (!spl->output_types)
		return ;
	spl->input_types = ft_calloc(sizeof(int *), (spl->cmd_count + 1));
	if (!spl->input_types)
		return ;
}

void	dup_to_iolists(t_spl *spl, int l, int i, int o)
{
	int	j;

	while (spl->ss[++l])
	{
		o = -1;
		i = -1;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<"))
			{
				spl->input_types[l][++i] = (!scmp(spl->ss[l][j], "<<"));
				spl->input_files[l][i] = ft_strdup(spl->ss[l][j + 1]);
				if (!spl->input_files[l][i])
					return ;
			}
			else if (!scmp(spl->ss[l][j], ">") || !scmp(spl->ss[l][j], ">>"))
			{
				spl->output_types[l][++o] = (!scmp(spl->ss[l][j], ">>"));
				spl->output_files[l][o] = ft_strdup(spl->ss[l][j + 1]);
				if (!spl->output_files[l][o])
					return ;
			}
		}
	}
}

void	init_redir_arrays(t_spl *spl)
{
	int	j;
	int	l;

	spl->redir_in = 0;
	spl->redir_out = 0;
	l = -1;
	while (spl->ss[++l])
	{
			j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<"))
				spl->redir_in++;
			else if (!scmp(spl->ss[l][j], ">") || !scmp(spl->ss[l][j], ">>"))
				spl->redir_out++;
		}
		spl->input_files[l] = ft_calloc(spl->redir_in + 1, sizeof(char *));
		spl->output_files[l] = ft_calloc(spl->redir_out + 1, sizeof(char *));
		spl->input_types[l] = ft_calloc(spl->redir_in + 1, sizeof(int));
		spl->output_types[l] = ft_calloc(spl->redir_out + 1, sizeof(int));
		if (!spl->input_files[l] || !spl->output_files[l]
			|| !spl->input_types[l] || !spl->output_types[l])
			return ;
	}
	dup_to_iolists(spl, -1, -1, -1);
}

int	validptr_afterredir(t_spl *spl, int *l, int *j)
{
	char	***s;

	s = spl->ss;
	if ((!scmp(s[(*l)][(*j)], "<") || !scmp(s[(*l)][(*j)], ">")
		|| !scmp(s[(*l)][(*j)], ">>") || !scmp(s[(*l)][(*j)], "<<")))
	{
		if ((!s[(*l)][(*j) + 1]) || *s[(*l)][(*j) + 1] == '|'
			|| (!scmp(s[(*l)][(*j) + 1], "<")
				|| !scmp(s[(*l)][(*j) + 1], ">")
					|| !scmp(s[(*l)][(*j) + 1], ">>")
						|| !scmp(s[(*l)][(*j) + 1], "<<")))
		{
			printf("parse error near '%s'\n", s[(*l)][(*j)]);
			spl->redir_bool = 0;
			return (1);
		}
	}
	return (0);
}

int	validate_redirs(t_spl *spl)
{
	int		l;
	int		j;

	l = -1;
	while (spl->ss[++l])
	{
		j = -1;
		while (spl->ss[l][++j])
		{
			if (*spl->ss[l][j] == '<' || *spl->ss[l][j] == '>')
			{	
				if (scmp(spl->ss[l][j], "<") && scmp(spl->ss[l][j], ">")
					&& scmp(spl->ss[l][j], ">>") && scmp(spl->ss[l][j], "<<"))
				{
					printf("parse error near '%s'\n", spl->ss[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				if (validptr_afterredir(spl, &l, &j))
					return (1);
				spl->redir_bool = 1;
			}
		}
	}
	return (0);
}
