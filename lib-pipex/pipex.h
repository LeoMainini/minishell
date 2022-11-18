/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 16:08:37 by leferrei          #+#    #+#             */
/*   Updated: 2022/11/18 16:40:02 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include "../libft/libft.h"


typedef struct s_vars {
	char	***cmds;
	int		fds[2];
	int		xfds[2];
	int		in_fd;
	int		out_fd;
	int		arg_count;
	int		here_doc;
	int		hd_fds[2];
	char	*path;
	char	**lines_in;
	int		status;
}			t_vars;


typedef struct	s_sim_args
{
    int		argc;
    char	**argv;
}	t_simargs;

typedef struct	s_ms
{
    int		ret;
	char	**rl_addr;
	int		builtins_outfd;
	int		system_outfd;
}   t_ms;

int		get_path(t_vars *data, int i, char *path, char **envp);
char	*get_p_pwd(char **envp);
char	**append_item(char **src, char *entry);
char	*ft_strfree_join(char **s1, char *s2);
void	free_and_exit(t_vars *data, int status, int _exit);
int		fork_lpipes_execute(t_vars *data, int i, char **envp, t_ms *data2);
int		check_cmd_error(char ***cmds, int i, int argc);
char	***get_commands(int argc, char **argv);
int		init_struct(t_vars *data, int argc, char **argv);
char	*clean_path(char **dirty_path, char *extra);
char	*find_shell_path(char **envp);
int		p_steps_back(t_vars *data, int i);
char	*join_chunks(char **str_chunks, char *sep, int limiter);
char	*absolute_to_relative_pwd(t_vars *data, int i, char *pwd);
char	***get_hd_commands(int argc, char **argv);
char	**parse_stdin_tolimit(char *limiter);
int		pipex(int argc, char **argv, char **envp, t_ms *data2);
int		exec_one(t_vars *data, int i, char **envp);
char	*get_next_line(int fd);

#endif