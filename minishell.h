/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2022/12/05 13:04:08 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "lib-pipex/pipex.h"
# include <stdio.h>
# include <unistd.h>

typedef struct s_spl
{
	char	***ss;
	char	*str;
	int		cmd_count;
	int		quotebool;
	int		redir_bool;
	char	***input_files;
	char	***output_files;
	int		**input_types;
	int		**output_types;
	int		redir_in;
	int		redir_out;
}	t_spl;

typedef struct s_cmdd
{
	int		in_fd;
	int		out_fd;
	char	**args;
}	t_cmdd;

char	**alloc_envmem(char **envs, int offset);
void	alloc_redir_arrays(t_spl *spl);
int		arg_count(t_spl *spl, char *s, int l);
t_spl	cmd_split(char *s);
int		change_dir(t_cmdd *argd, t_ms *data, int before_pipe);
int		echo(t_cmdd *argd, t_ms *data);
int		env(t_cmdd *argd, t_ms *data);
void	exit_status(int status, t_ms *data, unsigned int print_exit);
int		exit_shell(t_cmdd *argd, t_ms *data, int before_pipe);
int		export(t_cmdd *argd, t_ms *data, int before_pipe);
char	**duplicate_envp(char **envs, int offset, int freeable);
void	dupwithoutredirs(t_spl *spl);
int		ft_argsize(char *s, int i);
void	ft_parsing(char *str);
char	**get_env(char *env, t_ms *data);
char	*get_pwd(void);
char	**get_sep_env_values(char *str);
void	init_redir_arrays(t_spl *spl);
void	init_spl(t_spl *spl);
void		interpret_strings(char **strs, t_ms *data);
int		isvalidcmd(char *s, t_spl *spl);
void	manage_redirs(t_spl *spl);
int		pwd(t_cmdd *argd, t_ms *data);
int		set_ret_return(t_ms *data, int code);
void	sighandler(int signum);
char	*separate_redirs(char *s);
int		set_pwd(t_ms *data);
int		unset(t_cmdd *argd, t_ms *data, int before_pipe);
int		validate_redirs(t_spl *spl);
t_ms	*get_struct(t_ms **data);
t_spl	*get_cmdsplit(t_spl *cmdsplit);
int		handle_redirections(int i);
int		*perform_hd_chain(t_ms *data);
int		check_builtin(char *cmd);
int		handle_b_in(int i, t_spl *spl);
int		handle_b_out(int i, t_spl *spl, int success);
int		handle_b_redirections(int i);
int		pre_exec_prep(char ***cmd_argvs, int k, int pip[2], t_cmdd *cmds);
int		open_outfile(t_spl *spl, int i, int j);
int		check_cmd_executable(char *cmd);
char	*get_executable_path(t_ms *data, char *cmd, char **envp);
char	*create_hd_fp(void);
t_spl	*set_cmdsplit(char *read_line);
void	cleanup_exec_data(t_ms *data, t_spl *spl, char **read_line);
void	free_data(t_ms *data);
void	free_cmdsplit(t_spl *cspl);
void	free_inout_strs(char ****files, int ***types);
int		handle_in(int i, t_spl *spl);
int		handle_out(int i, t_spl *spl, int success);
int		handle_redirections(int i);
int		pre_sys_exec_prep(int in_fd, int out_fd, int i, char ***cmd_argv);
int		strs_to_fd(char **array, int fd);
int		handle_hd(t_ms *data, char *limit);
int		*perform_hd_chain(t_ms *data);
void	select_builtin(int i, t_ms *data, t_cmdd *cmds, int nil);
int		execute_builtin(char ***cmd_argvs, int k, t_ms *data, int pip[2]);
void	exec_child_pid(int in_fd, int out_fd, int i, char ***cmd_argv);
int		exec_sys_func(char ***cmd_argv, int *i, int pip[2]);
int		*save_pid(int **pids, int new_pid, int reset);
void	split_inter(t_spl *spl, int i);
void	await_pid_returns(t_ms *data, int *pids, t_spl *spl, int i);
void	init_data(int argc, char **argv, t_ms **data, char **envp);
int		handle_exec_data(char **read_line, t_ms *data, t_spl **spl);

#endif