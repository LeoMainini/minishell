/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/25 15:22:49 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//TODO: > redirections, >> redir append
//TODO: < redir file to stdin, << stdin to stdin
//TODO: "" interprets vars, '' text only,
//TODO: get command, llok for 2 quotes if yes - 1 arg, if no - stderr
//TODO: fix pipex to work with only 1 command

void    ft_parsing(char *str)
{
    int i;
    int pipctr;
    int dqtctr;
    int sqtctr;

    i = -1;
    pipctr = 0;
    dqtctr = 0;
    sqtctr = 0;
    while (str[++i])
    {
        if (str[i] == '|' && ((sqtctr == 0 && dqtctr == 0) || sqtctr % 2 == 0 || dqtctr % 2 == 0))
            pipctr++;
    }
    if (pipctr)
        cmdsplit(str);
}

char    **cmdsplit(char *str)
{

}
