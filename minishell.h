/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcarreir <bcarreir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/10 16:36:41 by bcarreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "lib-pipex/pipex.h"
# include <stdio.h>
# include <unistd.h>

typedef struct	s_ms
{
    int		ret;
	char	**rl_addr;
	int		builtins_outfd;
	int		system_outfd;
}   t_ms;

typedef struct	s_spl
{
    char    ***ss;
    int     cmd_count;
    int     quotebool;
}   t_spl;

typedef struct	s_cmdd
{
    int		in_fd;
    int		out_fd;
    char	**args;
}	t_cmdd;

typedef struct	s_sim_args
{
    int		argc;
    char	**argv;
}	t_simargs;


char	**alloc_envmem(char **envs, int	offset);
char	***cmd_split(char *s);
int     change_dir(t_cmdd *argd, t_ms *data, int before_pipe);
int		echo(t_cmdd *argd, t_ms *data);
int		env(t_cmdd *argd, t_ms *data);
void	exit_status(int status, char **line);
int		exit_shell(t_cmdd *argd, t_ms *data, int before_pipe);
int		export(t_cmdd *argd, t_ms *data, int before_pipe);
char	**duplicate_envp(char **envs, int offset);
void    ft_parsing(char *str);
char	*ft_strfree_join(char **s1, char *s2);
char	**get_env(char *env, t_ms *data);
char	*get_pwd();
char	**get_sep_env_values(char *str);
int     interpret_strings(t_cmdd *argd, t_ms *data);
int     pwd(t_cmdd *argd, t_ms *data);
int		set_ret_return(t_ms *data, int code);
void	sighandler(int signum);
int     set_pwd(t_ms *data);
int	    unset(t_cmdd *argd, t_ms *data, int before_pipe);

#endif