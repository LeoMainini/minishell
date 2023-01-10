/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2023/01/10 16:31:32 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>

typedef struct s_ms
{
	int			ret;
	char		**rl_addr;
	int			builtins_outfd;
	int			system_outfd;
	char		*path;
	int			in_child;
	int			*pids;
	int			pip[2];
	int			*hds;
	int			pids_written;
}	t_ms;

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
char	**duplicate_envp(char **envs, int offset, int freeable);
char	**get_env(char *env, t_ms *data);
char	**get_sep_env_values(char *str);
char	**parse_stdin_tolimit(char *limiter);
char	*create_hd_fp(void);
char	*find_shell_path(char **envp);
char	*get_env_value(char *env_name, t_ms *data);
char	*get_executable_path(t_ms *data, char *cmd, char **envp);
char	*get_pwd(void);
char	*join_chunks(char **str_chunks, char *sep, int limiter);
char	*rel_to_abs_pwd(t_cmdd *argd, int i, char *pwd);
char	*remove_char(char *str, int index);
char	*replace_name(char *str, char *env, int i, int j);
char	*separate_redirs(char *s);
int		*perform_hd_chain(t_ms *data);
int		*perform_hd_chain(t_ms *data);
int		*save_pid(int **pids, int new_pid, int reset, t_ms *data);
int		arg_count(t_spl *spl, char *s, int l);
int		change_dir(t_cmdd *argd, t_ms *data, int before_pipe);
int		check_builtin(char *cmd);
int		check_cmd_executable(char *cmd);
int		check_folder(t_cmdd *argd, int i);
int		echo(t_cmdd *argd, t_ms *data);
int		env(t_cmdd *argd, t_ms *data);
int		exec_sys_func(char ***cmd_argv, int *i, int pip[2]);
int		execute_builtin(char ***cmd_argvs, int k, t_ms *data, int pip[2]);
int		exit_shell(t_cmdd *argd, t_ms *data, int before_pipe);
int		export(t_cmdd *argd, t_ms *data, int before_pipe);
int		ft_argsize(char *s, int i);
int		get_index(char	*env);
int		handle_b_in(int i, t_spl *spl);
int		handle_b_out(int i, t_spl *spl, int success);
int		handle_b_redirections(int i);
int		handle_exec_data(char **read_line, t_ms *data, t_spl **spl);
int		handle_hd(t_ms *data, char *limit);
int		handle_in(int i, t_spl *spl);
int		handle_out(int i, t_spl *spl, int success);
int		handle_redirections(int i);
int		handle_redirections(int i);
int		init_data(int argc, char **argv, t_ms **data, char **envp);
int		is_alphastr(char *str);
int		isvalidcmd(char *s, t_spl *spl);
int		open_outfile(t_spl *spl, int i, int j);
int		pre_exec_prep(char ***cmd_argvs, int k, int pip[2], t_cmdd *cmds);
int		pre_sys_exec_prep(int in_fd, int out_fd, int i, char ***cmd_argv);
int		print_sorted_envs(int fd);
int		pwd(t_cmdd *argd, t_ms *data);
int		replace_value_str(char **str, int i, t_ms *data, int j);
int		set_pwd(t_ms *data);
int		set_ret_return(t_ms *data, int code);
int		steps_back(t_cmdd *argd, int i);
int		strs_to_fd(char **array, int fd);
int		unset(t_cmdd *argd, t_ms *data, int before_pipe);
int		validate_redirs(t_spl *spl);
t_ms	*get_struct(t_ms **data);
t_spl	*get_cmdsplit(t_spl *cmdsplit);
t_spl	*set_cmdsplit(char *read_line);
t_spl	cmd_split(char *s);
void	alloc_redir_arrays(t_spl *spl);
void	await_pid_returns(t_ms *data, int *pids, t_spl *spl, int i);
void	check_free_zeroout(void **ptr);
int		cleanup_exec_data(t_ms *data, t_spl *spl, char **read_line);
void	dupwithoutredirs(t_spl *spl);
void	exec_child_pid(int in_fd, int out_fd, int i, char ***cmd_argv);
void	exit_status(int status, t_ms *data, unsigned int print_exit);
void	free_cmdarray(t_spl *spl);
void	free_cmdsplit(t_spl *cspl, t_ms *data);
void	free_data(t_ms *data);
void	free_inout_strs(char ****files, int ***types);
void	ft_parsing(char *str);
void	ft_str_swap(char ***s1, char ***s2);
void	init_redir_arrays(t_spl *spl);
void	init_spl(t_spl *spl);
void	interpret_strings(char **strs, t_ms *data);
void	manage_redirs(t_spl *spl);
void	print_free_3darray(char ****str_array, int fd);
void	select_builtin(int i, t_ms *data, t_cmdd *cmds, int nil);
void	sighandler(int signum);
int		split_inter(t_spl *spl, int i);
void	free_hd_pid_mem(t_ms *data, t_spl *spl, char *file_path);
void	free_split_inter_data(t_spl *spl, char **split_out, int i, int toggle);
char	**alloc_result_mem( t_spl *spl, int i, char **split_out);
void	cmd_sighandler(int	signum);

#endif
