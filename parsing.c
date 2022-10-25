/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/25 14:38:59 by leferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//> redirections, >> redir append
//< redir file to stdin, << stdin to stdin
//"" interprets vars, '' text only,
// get command, llok for 2 quotes if yes - 1 arg, if no - stderr

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