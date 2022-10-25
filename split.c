/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/25 15:22:49 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "libft.h"

static int	ft_wordcount(char const *s, char c)
{
	int	wc;
    int i;
    int dqtctr;
    int sqtctr;

    dqtctr = 0;
    sqtctr = 0;

	wc = 0;
    i = -1;
	while (s[++i])
	{
		 if (s[i] == '|' && ((sqtctr % 2 == 0 || sqtctr % 2 == 2)
            && (dqtctr % 2 == 0 || dqtctr % 2 == 2)))
        {
            wc++;
        }
	}
	return (wc);
}

static void	*ft_dealmem(char **a, int j, char const *str, char c)
{
	char	**b;
	int		cc;

	b = a;
	cc = 0;
	while (*str && *str != c)
	{
		str++;
		cc++;
	}
	b[j] = (char *)malloc(sizeof(char) * (cc + 1));
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
	int		j;
	int		k;
	int		wc;

	if (!s)
		return (NULL);
	wc = ft_wordcount(s, c);
	a = (char **)malloc(sizeof(char *) * (wc + 1));
	if (!a)
		return (NULL);
	j = 0;
	while (j != wc)
	{
		while (c == *s && *s)
			s++;
		ft_dealmem(a, j, s, c);
		k = 0;
		while (*s && c != *s)
			a[j][k++] = (char)*s++;
		a[j][k] = '\0';
		j++;
	}
	a[j] = NULL;
	return (a);
}