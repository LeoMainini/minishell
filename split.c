/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/24 17:17:48 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// > OPEN IN REWRITE MODE
// >> OPEN IN APPEND MODE (NO DELETE)
// < SEND FILE TO STDIN
// << <lim> SEND STDIN TO STDIN OF COMMAND UNTIL <lim> IS FOUND
// manipulate ***char to move redirs+I/Ofiles to I/O arrays in order/types

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
	int	k;

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
			k = j + 1;
			while(s[k] && ft_isspace(s[k]))
				k++;
			if (!s[k] || s[k]== '|' || s[j+1] == '|' || j == 0)
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
	spl->ss = NULL;
	spl->cmd_count = 0;
	spl->quotebool = 1;
	spl->redir_bool = 0;
	spl->input_files = NULL;
	spl->input_types = NULL;
	spl->output_files = NULL;
	spl->output_types = NULL;
	spl->redir_in = 0;
	spl->redir_out = 0;
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

int	ft_argspercmd(t_spl *spl, char *s, int l)
{
	static int	i;
	char		q;
	int			argc;
	spl->quotebool = 0;

	argc = 0;
	
	if (!l)
	{
		i = 0;
	}
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
		{
			if (spl->cmd_count)	
				spl->quotebool = 0;
			i++;
		}	
		if (s[i] && s[i] != '|')
		{
			if (!spl->quotebool)
				argc++;
			if (s[i] && (s[i] == 34 || s[i] == 39))
			{
				q = s[i];
				i++;
				while (s[i] && s[i] != q)
					i++;
				if (s[i] && s[i] == q)
				{
					i++;
					spl->quotebool = 1;
					continue ;
				}
			}
			spl->quotebool = 0;
			if (s[i] && s[i] != '|' && !ft_isspace(s[i]))
				i++;

		}
		if (s[i] && s[i] == '|')
		{
			i++;
			break ;
		}
	}
	// printf("args per cmd %d\n", argc);
	return (argc);
}

int	ft_argsize(char *s, int i)
{
	char	q;
	int		j;

	j = i;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			q = s[i];
			i++;
			while (s[i] && s[i] != q)
				i++;
			if (s[i] && s[i] == q)
			{
				i++;
				continue ;
			}
		}
		else if (s[i] && !ft_isspace(s[i]) && s[i] != '|')
			i++;
		else if (s[i] && (ft_isspace(s[i]) || s[i] == '|'))
			break ;
	}
	// printf("argsize %d, s is %s \n", i - j, s + i);
	return (i - j);
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
		}
		i++;			
	}
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
	printf("aux is %s\n", aux);
	return (aux);
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
				if (ft_strcmp(s[l][j], "<") && ft_strcmp(s[l][j], ">") && ft_strcmp(s[l][j], ">>") && ft_strcmp(s[l][j], "<<"))
				{
					printf("parse error near '%s'\n", s[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				if ((!ft_strcmp(s[l][j], "<") || !ft_strcmp(s[l][j], ">") || !ft_strcmp(s[l][j], ">>") || !ft_strcmp(s[l][j], "<<"))
						&& ((!s[l][j + 1]) || *s[l][j + 1] == '|' 
							|| (!ft_strcmp(s[l][j + 1], "<") || !ft_strcmp(s[l][j + 1], ">")
								|| !ft_strcmp(s[l][j + 1], ">>") || !ft_strcmp(s[l][j + 1], "<<"))))
				{
					printf("parse error near '%s'\n", s[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				else if (!ft_strcmp(s[l][j], "<") || !ft_strcmp(s[l][j], "<<"))
					spl->redir_in++;
				else if (!ft_strcmp(s[l][j], ">") || !ft_strcmp(s[l][j], ">>"))
					spl->redir_out++;
				spl->redir_bool = 1;
			}
		}
	}
	return (0);
}

void	init_redir_arrays(t_spl *spl)
{
	//ALLOCATION OF **CHAR MISSING AFTER REDIR COUNT
	int	l;
	int	j;
	int	i;
	int	o;

	spl->output_files = ft_calloc(sizeof(char*), (spl->cmd_count + 1));
	if (!spl->output_files)
		return ;
	spl->input_files = ft_calloc(sizeof(char*), (spl->cmd_count + 1));
	if (!spl->input_files)
		return ;
	spl->output_types = ft_calloc(sizeof(int), (spl->cmd_count + 1));
	if (!spl->output_types)
		return ;
	spl->input_types = ft_calloc(sizeof(int), (spl->cmd_count + 1));
	if (!spl->input_types)
		return ;
	l = -1;
	j = -1;
	i = -1;
	o = -1;
	while (spl->ss[++l])
	{
		while (spl->ss[l][++j])
		{
			if (!ft_strcmp(spl->ss[l][j], "<") || !ft_strcmp(spl->ss[l][j], "<<"))
			{
				spl->input_files[++i] = ft_strdup(spl->ss[l][j + 1]);
				spl->input_types[i] = (ft_strcmp(spl->ss[l][j], "<<")!= 0);
			}
			else if (!ft_strcmp(spl->ss[l][j], ">") || !ft_strcmp(spl->ss[l][j], ">>"))
			{
				spl->output_files[++o] = ft_strdup(spl->ss[l][j + 1]);
				spl->output_types[o] = (ft_strcmp(spl->ss[l][j], ">>") != 0);
			}
		}
	}
}

t_spl	cmd_split(char *s)
{
	t_spl	spl;
	char	*aux;
	char	q;
	int		i;
	int		j;
	int		l;
	int		k;

	spl.ss = NULL;
	if (!s)
		return (spl);
	init_spl(&spl);
	if (isvalidcmd(s, &spl))
		return (spl);
	aux = s;
	s = separate_redirs(aux);
	spl.ss = (char ***)ft_calloc(sizeof(char **), (spl.cmd_count + 1));
	if (!spl.ss)
		return (spl);
	l = 0;
	while (l < spl.cmd_count)
	{
		spl.ss[l] = ft_calloc(sizeof(char*), (ft_argspercmd(&spl, s, l) + 1));
		if (!spl.ss[l])
			return (spl);
		l++;
	}
	i= 0;
	j = 0;
	l = 0;
	k = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
				i++;
		if (!s[i])
			break;
		if (s[i] && s[i] == '|')
		{
			l++;
			j = 0;
			i++;
			while (s[i] && ft_isspace(s[i]))
				i++;
		}
		if (s[i] && s[i] != '|')
		{
			k = ft_argsize(s, i);
			// printf("split l = %d j = %d k = %d i = %d\n", l, j, k, i);
			spl.ss[l][j] = ft_calloc(sizeof(char), (k + 1));
			if (!spl.ss[l][j])
				return (spl);
		}
		k = 0;
		while (s[i])
		{
			while (s[i] && !ft_isspace(s[i]) && s[i] != '|')
			{
				if (s[i] && (s[i] == 34 || s[i] == 39))
				{
					q = s[i];
					spl.ss[l][j][k++] = s[i++];
					while (s[i] && s[i] != q)
						spl.ss[l][j][k++] = s[i++];
					if (s[i] && s[i] == q)
					{
						spl.ss[l][j][k++] = s[i++];
						continue ;
					}
				}
				spl.ss[l][j][k++] = s[i++];
			}
		// printf("K is %d i is %d\n", k, i);
			if (s[i] && (ft_isspace(s[i]) || s[i] == '|'))
				break ;
		}
		spl.ss[l][j][k] = '\0';
		j++;
	}
	free (s);
	if (validate_redirs(&spl))
		return (spl);
	if (spl.redir_bool)
		init_redir_arrays(&spl);

		
	//printing
	// l = 0;
	// j = 0;
	// while (spl.ss[l])
	// {
	// 	j = 0;
	// 	while (spl.ss[l] && spl.ss[l][j])
	// 	{
	// 		printf("%d %d %s\n",l,j, spl.ss[l][j]);
	// 		j++;
	// 	}
	// 	l++;
	// }
	return (spl);
}
