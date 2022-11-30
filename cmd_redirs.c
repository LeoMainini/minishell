/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 17:10:39 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/30 17:15:01 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmdarray(t_spl *spl)
{
	int	i;
	int	j;

	i = -1;
	while (spl->ss && spl->ss[++i])
	{
		j = -1;
		while (spl->ss[i][++j])
			free(spl->ss[i][j]);
		free(spl->ss[i]);
	}
	free(spl->ss);
	spl->ss = NULL;
}

void	get_nbr_redirs_and_alloc(t_spl *spl, char ****aux)
{
	int	i;
	int	j;
	int	l;

	l = -1;
	while (spl->ss[++l])
	{
		i = 0;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<")
				|| !scmp(spl->ss[l][j], "<<")
					|| !scmp(spl->ss[l][j], ">")
						|| !scmp(spl->ss[l][j], ">>"))
				j++;
			else
				i++;
		}
		(*aux)[l] = ft_calloc(i + 1, sizeof(char *));
		if (!(*aux)[l])
			return ;
	}
}

void	duptoaux(t_spl *spl, char ****aux)
{
	int		l;
	int		j;
	int		i;

	l = -1;
	while (spl->ss[++l])
	{
		i = -1;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<")
				|| !scmp(spl->ss[l][j], "<<")
					|| !scmp(spl->ss[l][j], ">")
						|| !scmp(spl->ss[l][j], ">>"))
					j++;
			else
			{
				(*aux)[l][++i] = ft_strdup(spl->ss[l][j]);
				if (!(*aux)[l][i])
					return ;
			}
		}
	}
}

void	dupwithoutredirs(t_spl *spl)
{
	char	***aux;

	aux = ft_calloc(spl->cmd_count + 1, sizeof(char **));
	if (!aux)
		return ;
	get_nbr_redirs_and_alloc(spl, &aux);
	duptoaux(spl, &aux);
	free_cmdarray(spl);
	spl->ss = aux;
}


int	validate_redirs(t_spl *spl)
{
	char	***s;
	int		l;
	int		j;

	s = spl->ss;
	l = -1;
	while (s[++l])
	{
		j = -1;
		while (s[l][++j])
		{
			if (*s[l][j] == '<' || *s[l][j] == '>')
			{	
				if (scmp(s[l][j], "<") && scmp(s[l][j], ">")
					&& scmp(s[l][j], ">>") && scmp(s[l][j], "<<"))
				{
					printf("parse error near '%s'\n", s[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				if ((!scmp(s[l][j], "<") || !scmp(s[l][j], ">")
					|| !scmp(s[l][j], ">>") || !scmp(s[l][j], "<<")))
				{
					if ((!s[l][j + 1]) || *s[l][j + 1] == '|'
						|| (!scmp(s[l][j + 1], "<")
							|| !scmp(s[l][j + 1], ">")
								|| !scmp(s[l][j + 1], ">>")
									|| !scmp(s[l][j + 1], "<<")))
					{
						printf("parse error near '%s'\n", s[l][j]);
						spl->redir_bool = 0;
						return (1);
					}
				}
				spl->redir_bool = 1;
			}
		}
	}
	return (0);
}
