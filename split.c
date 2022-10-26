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

//MISSING QUOTE VALIDATION "'"

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
	if (str[i] == '|')
	{
		
		if ((sqtctr % 2 == 0 || sqtctr % 2 == 2) && (dqtctr % 2 == 0 || dqtctr % 2 == 2))
			return (1);
	}
	return (0);
}

int	isvalidcmd(char *s)
{
	int	j;
    int dqtctr;
    int sqtctr;

    dqtctr = 0;
    sqtctr = 0;
	j = -1;
	while (s[++j])
	{
		if (s[j] == 34)
			dqtctr++;
		if (s[j] == 39)
			sqtctr++;
		if (s[j] == '|' && isvalidpipe(s, j))
		{
			if (s[j+1] == '|')
			{
				printf("parse error near `|'\n");
				return (1);
			}
		}
	}
	if ((sqtctr % 2 != 0 && sqtctr % 2 != 2) || (dqtctr % 2 != 0 && dqtctr % 2 != 2))
	{
		printf("parse error, invalid number of quotes\n");
		return (1);
	}
	return (0);
}

static int	ft_cmdcount(char *s)
{
	int	cmdnr;
    int i;

	cmdnr = 0;
	if (*s)
		cmdnr++;
    i = -1;
	while (s[++i])
	{
		if (s[i] == '|' && isvalidpipe(s, i))
			cmdnr++;
	}
	return (cmdnr);
}

static int	ft_dealmem(char **a, char *str)
{
	char		**b;
	static int	i;

	b = a;
	while (str[i])
	{
		if (str[i] == '|' && isvalidpipe(str, i))
		{
			i++;
			break ;
		}
		i++;
	}
	*b = malloc(sizeof(char) * (i + 1));
	if (!b)
		return (1);
	return (0);
}

char	**cmd_split(char *s)
{
	char	**a;
	int		i;
	int		j;
	int		k;
	int		cmdnr;

	if (!s)
		return (NULL);
	if (isvalidcmd(s))
		return (NULL);
	cmdnr = ft_cmdcount(s);
	a = (char **)malloc(sizeof(char *) * (cmdnr + 1));
	if (!a)
		return (NULL);
	j = 0;
	i = 0;
	while (j != cmdnr)
	{
		while (s[i] && (j && s[i] == '|'))
			i++;
		if (ft_dealmem(&a[j], s))
			return (NULL);
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

	j = 0;
	while (a[j])
		printf("%s\n", a[j++]);
	return (a);
}