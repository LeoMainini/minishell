/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benmonico <benmonico@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/26 14:36:01 by benmonico        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isvalidpipe(char *str, int i)
{
	int	j;
    int dqtctr;
    int sqtctr;

    dqtctr = 0;
    sqtctr = 0;
	j = 0;
	while (str[j] && j < i)
	{
		if (str[j] == 34)
			dqtctr++;
		if (str[j] == 39)
			sqtctr++;
		j++;
	}
	if (str[i] == '|' && ((sqtctr % 2 == 0 || sqtctr % 2 == 2)
		&& (dqtctr % 2 == 0 || dqtctr % 2 == 2)))
		return (1);
	return (0);
}

static int	ft_cmdcount(char const *s, char c)
{
	int	cmdnr;
    int i;
	cmdnr = 0;
    i = -1;
	while (s[++i])
	{
		if (s[i] == '|' && isvalidpipe(s, i))
			cmdnr++;
	}
	return (cmdnr);
}

static void	*ft_dealmem(char **a, int j, char const *str, char c)
{
	char	**b;
	int		i;

	b = a;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|' && isvalidpipe(str, i))
			break ;
		i++;
	}
	b[j] = (char *)malloc(sizeof(char) * (i + 1));
	if (!b[j])
	{
		while (j--)
			free(b[j]);
		free(b);
		return (NULL);
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**a;
	int		i;
	int		j;
	int		k;
	int		cmdnr;

	if (!s)
		return (NULL);
	cmdnr = ft_cmdcount(s, c);
	a = (char **)malloc(sizeof(char *) * (cmdnr + 1));
	if (!a)
		return (NULL);
	j = 0;
	i = 0;
	while (j != cmdnr)
	{
		while (s[i] && (ft_isspace(s[i]) || (j && s[i] == '|' && isvalidpipe(s, i))))
			i++;
		ft_dealmem(a, j, s, c);
		k = 0;
		while (s[i])
		{
			if (s[i] == '|' && isvalidpipe(s, i))
				break ;
			a[j][k] = s[i];
			k++;
			i++;
		}
		a[j][k] = '\0';
		j++;
	}
	a[j] = NULL;
	return (a);
}