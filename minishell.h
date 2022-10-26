/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 15:55:01 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/25 17:28:10 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>

typedef struct s_ms
{
    int ret;
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
char	*get_pwd();
int		set_ret_return(t_ms *data, int code);
void	sighandler(int signum);

#endif