/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ben <ben@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/08 13:34:36 by ben              ###   ########.fr       */
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
			if (s[j+1] == '|' || j == 0)
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
			if (!s[j][i])
				return (1);
			else
				break ;
		}
		j++;
	}
	return (0);
}

static int	ft_alloc_arg(char **b,char *str)
{
	static int	i;

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
	printf("%d arg size\n", i);

	return (0);
}

static int	ft_alloc_cmd(char ***b, char *str)
{
	static int	i;
	int			j;

	j = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (str[i] && str[i] == '|' && isvalidpipe(str, i))
		{
			i++;
			break ;
		}
		else if (str[i])
		{
			if (str[i] == 34 || str[i] == 39)
				ffquotedtext(NULL, str, &i, str[i]);
			while(str[i] && !ft_isspace(str[i]))
				i++;
			j++;
		}
	}
	*b = malloc(sizeof(char*) * (j + 1));
	if (!b)
		return (1);
	printf("%d cmd size\n", j);
	return (0);
}

char	***cmd_split(char *s)
{
	t_spl	spl;
	int		i;
	int		j;
	int		l;
	int		k;

	if (!s)
		return (NULL);
	init_spl(&spl);
	if (isvalidcmd(s, &spl))
		return (NULL);
	spl.ss = (char ***)malloc(sizeof(char **) * (spl.cmd_count + 1));
	if (!spl.ss)
		return (NULL);
	printf("%d cmds\n", spl.cmd_count);
	l = 0;
	j = 0;
	i = 0;
	while (l != spl.cmd_count)
	{
		if (j == 0)
		{	
			ft_alloc_cmd(&spl.ss[l], s);
			l++;
		}
		else if (s[i] && s[i] == '|' && j > 0)
		{
			i++;
			l++;
			j=0;
			ft_alloc_cmd(&spl.ss[l], s);
		}
		else
		{
			while (s[i] && ft_isspace(s[i]))
				i++;
			if (s[i] && s[i] != '|')
				if (ft_alloc_arg(&spl.ss[l][j], s))
				{
					j++;
					return (NULL);
				}
		}
		k = 0;
		while (s[i])
		{
			if (s[i] == '|' && isvalidpipe(s, i))
				break ;
			spl.ss[l][j][k] = s[i];
			k++;
			i++;
		}
		spl.ss[l][j][k] = '\0';
	}
	spl.ss[l][j] = NULL;
	write(1, "end\n", 4);
	if (*spl.ss)
	{	
		if (spl.ss[l] && checkemptycmds(spl.ss[l]))
		{
			printf("parse error near. empty cmd '|'\n");
			return (NULL);
		}
	}
	j = 0;
	while (spl.ss[l][j])
		printf("%s\n", spl.ss[l][j++]);
	return (spl.ss);
}