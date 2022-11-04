/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/04 18:38:52 by ben              ###   ########.fr       */
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

int	isvalidcmd(char *s,  t_spl *spl)
{
	int	j;

	j = 0;
	if (*s)
		spl->cmd_count++;
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
		if (s[j] == '|')
		{
			if (s[j+1] == '|')
			{
				printf("parse error near `|'\n");
				return (1);
			}
			spl->cmd_count++;
		}
		j++;
	}
	return (0);
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

void	init_spl(t_spl *spl)
{
	spl->cmd_count = 0;
	spl->ss = NULL;
	spl->quotebool = 1;
}

int	checkemptycmds(char **s)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (s[j])
	{
		while (s[j][i])
		{
			while (s[j][i] && ft_isspace(s[j][i]))
				i++;
			if (!s[j][i] || (**s && **s == '|'))
				return (1);
			else
				break ;
		}
		j++;
	}
	return (0);
}

char	**cmd_split(char *s)
{
	t_spl	spl;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	init_spl(&spl);
	if (isvalidcmd(s, &spl))
		return (NULL);
	spl.ss = (char **)malloc(sizeof(char *) * (spl.cmd_count + 1));
	if (!spl.ss)
		return (NULL);
	j = 0;
	i = 0;
	while (j != spl.cmd_count)
	{
		while (s[i] && (j && s[i] == '|'))
			i++;
		if (ft_dealmem(&spl.ss[j], s))
			return (NULL);
		k = 0;
		while (s[i])
		{
			if (s[i] == '|' && isvalidpipe(s, i))
				break ;
			spl.ss[j][k] = s[i];
			k++;
			i++;
		}
		spl.ss[j][k] = '\0';
		j++;
	}
	spl.ss[j] = NULL;
	if (*spl.ss)
	{	
		if (*spl.ss && checkemptycmds(spl.ss))
		{
			printf("parse error near '|'\n");
			return (NULL);
		}
	}
	j = 0;
	while (spl.ss[j])
		printf("%s\n", spl.ss[j++]);
	return (spl.ss);
}