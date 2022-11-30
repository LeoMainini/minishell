/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2022/11/30 17:53:25 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "lib-pipex/pipex.h"
# include <stdio.h>
# include <unistd.h>

typedef struct	s_spl
{
    char    ***ss;
    char    *str;
    int     cmd_count;
    int     quotebool;
    int     redir_bool;
    char    ***input_files;
    char    ***output_files;
    int    **input_types;
    int    **output_types;
    int     redir_in;
    int     redir_out;
}   t_spl;

typedef struct	s_cmdd
{
    int		in_fd;
    int		out_fd;
    char	**args;
}	t_cmdd;


char	**alloc_envmem(char **envs, int	offset);
void	alloc_redir_arrays(t_spl *spl);
int	arg_count(t_spl *spl, char *s, int l);
t_spl   cmd_split(char *s);
int     change_dir(t_cmdd *argd, t_ms *data, int before_pipe);
int		echo(t_cmdd *argd, t_ms *data);
int		env(t_cmdd *argd, t_ms *data);
void	exit_status(int status, t_ms *data);
int		exit_shell(t_cmdd *argd, t_ms *data, int before_pipe);
int		export(t_cmdd *argd, t_ms *data, int before_pipe);
char	**duplicate_envp(char **envs, int offset, int freeable);
void	dupwithoutredirs(t_spl *spl);
int	ft_argsize(char *s, int i);
void    ft_parsing(char *str);
char	**get_env(char *env, t_ms *data);
char	*get_pwd();
char	**get_sep_env_values(char *str);
void	init_redir_arrays(t_spl *spl);
void	init_spl(t_spl *spl);
int     interpret_strings(char **strs, t_ms *data);
int	isvalidcmd(char *s, t_spl *spl);
void	manage_redirs(t_spl *spl);
int     pwd(t_cmdd *argd, t_ms *data);
int		set_ret_return(t_ms *data, int code);
void	sighandler(int signum);
char	*separate_redirs(char *s);
int     set_pwd(t_ms *data);
int	    unset(t_cmdd *argd, t_ms *data, int before_pipe);
int	validate_redirs(t_spl *spl);
t_ms	*get_struct(t_ms **data);
t_spl	*fetch_cmdsplit(t_spl *cmdsplit);
int		handle_redirections(int	i);
int	*perform_hd_chain(t_ms *data);

#endif