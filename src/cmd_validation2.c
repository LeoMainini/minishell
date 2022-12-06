/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_validation2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:38:23 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/06 15:39:57 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_redir_count(char *s, int *i)
{
	char	temp;
	int		redir_count;

	redir_count = 0;
	while (s[(*i)])
	{
		if (s[(*i)] == 34 || s[(*i)] == 39)
		{
			temp = s[(*i)++];
			while (s[(*i)] && s[(*i)] != temp)
				(*i)++;
			(*i)++;
			continue ;
		}
		if (s[(*i)] && (s[(*i)] == '<' || s[(*i)] == '>'))
		{
			while (s[(*i)] && (s[(*i)] == '<' || s[(*i)] == '>'))
				(*i)++;
			redir_count++;
			if (!s[(*i)])
				break ;
		}
		(*i)++;
	}
	return (redir_count);
}

void	strcpy_with_spaces(char *s, char **aux, int i, int j)
{
	char	temp;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			temp = s[i];
			(*aux)[j++] = s[i++];
			while (s[i] && s[i] != temp)
				(*aux)[j++] = s[i++];
			(*aux)[j++] = s[i++];
			continue ;
		}
		if (s[i] && (s[i] == '<' || s[i] == '>'))
		{
			(*aux)[j++] = ' ';
			while (s[i] && (s[i] == '<' || s[i] == '>'))
				(*aux)[j++] = s[i++];
			(*aux)[j++] = ' ';
			continue ;
		}
		(*aux)[j++] = s[i++];
	}
}

char	*separate_redirs(char *s)
{
	char	*aux;
	int		redir_count;
	int		i;

	i = 0;
	redir_count = get_redir_count(s, &i);
	if (!redir_count)
		return (NULL);
	aux = ft_calloc(sizeof(char), ((redir_count * 2) + i + 1));
	if (!aux)
		return (NULL);
	strcpy_with_spaces(s, &aux, 0, 0);
	return (aux);
}
