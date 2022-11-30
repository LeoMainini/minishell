/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:07:13 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/30 17:33:31 by bcarreir         ###   ########.fr       */
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
		while (s[(*k)] && ft_isspace(s[(*k)]))
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


char	*separate_redirs(char *s)
{
	char	*aux;
	char	temp;
	int		redir_count;
	int		i;
	int		j;

	redir_count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			temp = s[i++];
			while (s[i] && s[i] != temp)
				i++;
			i++;
			continue ;
		}
		if (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			while (s[i] && (s[i] == '<' || s[i] == '>'))
				i++;
			redir_count++;
			if (!s[i])
				break ;
		}
		i++;
	}
	if (!redir_count)
		return (NULL);
	aux = ft_calloc(sizeof(char), ((redir_count * 2) + i + 1));
	if (!aux)
		return (NULL);
	j = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			temp = s[i];
			aux[j++] = s[i++];
			while (s[i] && s[i] != temp)
				aux[j++] = s[i++];
			aux[j++] = s[i++];
			continue ;
		}
		if (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			aux[j++] = ' ';
			while (s[i] && (s[i] == '<' || s[i] == '>'))
				aux[j++] = s[i++];
			aux[j++] = ' ';
			continue ;
		}
		aux[j++] = s[i++];
	}
	aux[j] = '\0';
	return (aux);
}
