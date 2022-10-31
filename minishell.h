/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/31 17:09:53 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>

typedef struct s_ms
{
    int		ret;
	char	**rl_addr;
}   t_ms;

typedef struct s_cmdd
{
    int		in_fd;
    int		out_fd;
    char	**args;
}	t_cmdd;

int		change_dir(t_cmdd *argd, t_ms *data);
char	**cmd_split(char *s);
void    ft_parsing(char *str);
int		env(t_cmdd *argd, t_ms *data);
char	**duplicate_envp(char **envs, int offset);
int		export(t_cmdd *argd, t_ms *data);
char	**get_env(char *env, t_ms *data);
char	*get_pwd();
int		set_ret_return(t_ms *data, int code);
void	sighandler(int signum);
int     pwd(t_cmdd *argd, t_ms *data);
int     set_pwd(t_ms *data);
int	    unset(t_cmdd *argd, t_ms *data);
char	**alloc_envmem(char **envs, int	offset);
void	exit_status(int status, char **line);
int		exit_shell(t_cmdd *argd, t_ms *data);
int		echo(t_cmdd *argd, t_ms *data);
void	interpret_strings(t_cmdd *argd, t_ms *data);

#endif