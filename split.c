/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/30 16:46:06 by leferrei         ###   ########.fr       */
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
		i = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
		{
			spl->quotebool = 0;
			i++;
		}
		if (!s[i])	
			break ;
		if (s[i] && s[i] == '|')
		{
			i++;
			break ;
		}
		if (s[i] && s[i] != '|')
		{
			if (!spl->quotebool)
				argc++;
			spl->quotebool = 1;
			if (s[i] == 34 || s[i] == 39)
			{
				q = s[i++];
				while (s[i] && s[i] != q)
					i++;
				if (s[i] && s[i] == q)
				{
					i++;
					spl->quotebool = 1;
					continue ;
				}
			}
			if (s[i] && s[i] != '|' && !ft_isspace(s[i]))
				i++;

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
	// printf("argsize %d\n", i - j);
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
	// printf ("aux is %s\n", aux);
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
				if (scmp(s[l][j], "<") && scmp(s[l][j], ">") && scmp(s[l][j], ">>") && scmp(s[l][j], "<<"))
				{
					printf("parse error near '%s'\n", s[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				if ((!scmp(s[l][j], "<") || !scmp(s[l][j], ">") || !scmp(s[l][j], ">>") || !scmp(s[l][j], "<<"))
						&& ((!s[l][j + 1]) || *s[l][j + 1] == '|' 
							|| (!scmp(s[l][j + 1], "<") || !scmp(s[l][j + 1], ">")
								|| !scmp(s[l][j + 1], ">>") || !scmp(s[l][j + 1], "<<"))))
				{
					printf("parse error near '%s'\n", s[l][j]);
					spl->redir_bool = 0;
					return (1);
				}
				spl->redir_bool = 1;
			}
		}
	}
	return (0);
}

void	alloc_redir_arrays(t_spl *spl)
{
	//ALLOCATION OF **CHAR MISSING AFTER REDIR COUNT
	spl->output_files = ft_calloc(sizeof(char**), (spl->cmd_count + 1));
	if (!spl->output_files)
		return ;
	spl->input_files = ft_calloc(sizeof(char**), (spl->cmd_count + 1));
	if (!spl->input_files)
		return ;
	spl->output_types = ft_calloc(sizeof(int*), (spl->cmd_count + 1));
	if (!spl->output_types)
		return ;
	spl->input_types = ft_calloc(sizeof(int*), (spl->cmd_count + 1));
	if (!spl->input_types)
		return ;
}

void	init_redir_arrays(t_spl *spl)
{
	int	i;
	int	o;
	int	j;
	int	l;

	spl->redir_in = 0;
	spl->redir_out = 0;
	l = -1;
	while (spl->ss[++l])
	{
			j =-1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<"))
				spl->redir_in++;
			else if (!scmp(spl->ss[l][j], ">") || !scmp(spl->ss[l][j], ">>"))
				spl->redir_out++;
		}
		spl->input_files[l] = ft_calloc(spl->redir_in + 1, sizeof(char*));
		spl->output_files[l] = ft_calloc(spl->redir_out + 1, sizeof(char*));
		spl->input_types[l] = ft_calloc(spl->redir_in + 1, sizeof(int));
		spl->output_types[l] = ft_calloc(spl->redir_out + 1, sizeof(int));
		if (!spl->input_files[l] || !spl->output_files[l]
			|| !spl->input_types[l] || !spl->output_types[l])
			return ;
	}
	l = -1;
	while (spl->ss[++l])
	{
		o = -1;
		i = -1;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<"))
			{
				spl->input_files[l][++i] = ft_strdup(spl->ss[l][j + 1]);
				if (!scmp(spl->ss[l][j], "<"))
					spl->input_types[l][i] = 0;
				else if (!scmp(spl->ss[l][j], "<<"))
					spl->input_types[l][i] = 1;	
				if (!spl->input_files[l][i])
					return;
			}
			else if (!scmp(spl->ss[l][j], ">") || !scmp(spl->ss[l][j], ">>"))
			{
				spl->output_files[l][++o] = ft_strdup(spl->ss[l][j + 1]);
				if (!scmp(spl->ss[l][j], ">"))
					spl->output_types[l][o] = 0;
				else if (!scmp(spl->ss[l][j], ">>"))
					spl->output_types[l][o] = 1;	
				if (!spl->output_files[l][o])
					return;
			}
		}
	}

	
	//printing
	// l = -1;
	// while (spl->input_files[++l])
	// {
	// 	j = -1;
	// 	while (spl->input_files[l][++j])
	// 		printf("infiles %d %d %s mode = %d\n",l,j, spl->input_files[l][j], spl->input_types[l][j]);
	// }

	// l = -1;
	// while (spl->output_files[++l])
	// {
	// 	j = -1;
	// 	while (spl->output_files[l][++j])
	// 		printf("outfiles %d %d %s\n",l,j, spl->output_files[l][j]);
	// }
}

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

void	dupwithoutredirs(t_spl *spl)
{
	char	***aux;
	int		l;
	int		j;
	int		i;

	aux = NULL;
	aux = ft_calloc(spl->cmd_count + 1, sizeof(char**));
	if (!aux)
		return ;
	l = -1;
	while (spl->ss[++l])
	{
		i = 0;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<") 
				|| !scmp(spl->ss[l][j], ">")|| !scmp(spl->ss[l][j], ">>"))
				j++;
			else
				i++;
		}
		aux[l] = ft_calloc(i + 1, sizeof(char *));
		if (!aux[l])
			return ;
	}
	l = -1;
	while (spl->ss[++l])
	{
		i = -1;
		j = -1;
		while (spl->ss[l][++j])
		{
			if (!scmp(spl->ss[l][j], "<") || !scmp(spl->ss[l][j], "<<") 
				|| !scmp(spl->ss[l][j], ">")|| !scmp(spl->ss[l][j], ">>"))
					j++;
			else
			{
				aux[l][++i] = ft_strdup(spl->ss[l][j]);
				if (!aux[l][i])
					return ;
			}
		}
	}
	free_cmdarray(spl);
	spl->ss = aux;


}

int	verify_alloc_ss(char *s, t_spl *spl)
{
	int		l;

	init_spl(spl);
	if (!s)
		return (1);
	if (isvalidcmd(s, spl))
		return (1);
	spl->str = separate_redirs(s);
	if (!spl->str)
		spl->str = ft_strdup(s);
	spl->ss = ft_calloc(sizeof(char **), (spl->cmd_count + 1));
	if (!spl->ss)
		return (1);
	l = 0;
	while (l < spl->cmd_count)
	{
		spl->ss[l] = ft_calloc(sizeof(char*), (ft_argspercmd(spl, spl->str, l) + 1));
		if (!spl->ss[l])
			return (1);
		l++;
	}
	return (0);
}

void	manage_redirs(t_spl *spl)
{
	if (validate_redirs(spl))
		return ;
	if (spl->redir_bool)
	{
		alloc_redir_arrays(spl);
		init_redir_arrays(spl);
		dupwithoutredirs(spl);	
	}
}

int	get_new_arg(t_spl *spl, int *i, int *l, int *j)
{
	int	k;

	k = 0;
	while (spl->str[(*i)] && ft_isspace(spl->str[(*i)]))
				(*i)++;
		if (!spl->str[(*i)])
			return (1);
		if (spl->str[(*i)] && spl->str[(*i)] == '|')
		{
			(*l)++;
			(*j) = 0;
			(*i)++;
			while (spl->str[(*i)] && ft_isspace(spl->str[(*i)]))
				(*i)++;
		}
		if (spl->str[(*i)] && spl->str[(*i)] != '|')
		{
			k = ft_argsize(spl->str, (*i));
			spl->ss[(*l)][(*j)] = ft_calloc(sizeof(char), (k + 1));
			if (!spl->ss[(*l)][(*j)])
				return (1);
		}
		return (0);
}

void	copytoarg(t_spl *spl, int *i, int*j, int *l)
{
	int	k;
	int	q;

	k = 0;
		while (spl->str[(*i)])
		{
			while (spl->str[(*i)] && !ft_isspace(spl->str[(*i)]) && spl->str[(*i)] != '|')
			{
				if (spl->str[(*i)] && (spl->str[(*i)] == 34 || spl->str[(*i)] == 39))
				{
					q = spl->str[(*i)];
					spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
					while (spl->str[(*i)] && spl->str[(*i)] != q)
						spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
					if (spl->str[(*i)] && spl->str[(*i)] == q)
					{
						spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
						continue ;
					}
				}
				spl->ss[(*l)][(*j)][k++] = spl->str[(*i)++];
			}
			if (spl->str[(*i)] && (ft_isspace(spl->str[(*i)]) || spl->str[(*i)] == '|'))
				break ;
		}
		(*j)++;
}

t_spl	cmd_split(char *s)
{
	t_spl	spl;
	int		i;
	int		j;
	int		l;

	j = 0;
	l = 0;
	i = 0;
	verify_alloc_ss(s, &spl);
	while (spl.str[i])
	{
		if (get_new_arg(&spl, &i, &l, &j))
			break ;
		copytoarg(&spl, &i, &j, &l);
	}
	manage_redirs(&spl);
	free(spl.str);
	spl.str = NULL;
		
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
