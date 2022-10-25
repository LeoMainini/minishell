/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leferrei <leferrei@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 18:45:17 by bcarreir          #+#    #+#             */
/*   Updated: 2022/10/25 16:17:34 by leferrei         ###   ########.fr       */
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

    pipctr = 0;
    dqtctr = 0;
    sqtctr = 0;
    i = -1;
    while (str[++i])
    {
        if (str[i] == '|' && ((sqtctr % 2 == 0 || sqtctr % 2 == 2) && (dqtctr % 2 == 0 || dqtctr % 2 == 2)))
        {
            cmdsplit(str, i);
            pipctr++;
        }
    }
}

char    **cmdsplit(char *str, int z)
{
    static char **str_arr;
    char        **aux_arr;
    static int  i;
    int         j;
    int         k;

    if (!str_arr)
        i = 2;
    aux_arr = str_arr;
    str_arr = (char **)malloc(sizeof(char *) * (++i));
    if (!str_arr)
        return (NULL);
    j = 0;
    while (aux_arr[j] && !ft_strchr(aux_arr[j], '|'))
    {
        str_arr[j] = ft_strdup(aux_arr[j]);
        if (!str_arr[j])
            return (NULL);
    }
    str_arr[j] = (char*)malloc(sizeof())
}

char    *strspacetrim(char *str, int z)
{
    char    *strtrim;
    int i;

    while (ft_isspace(str[i]))
        i++;
    while (ft_isspace(str[z]))
        z--;
    strtrim = ft_calloc(z - i + 1, 1);
    if (!strtrim)
        return (NULL);
    ft_memmove(strtrim, str[i], z - i);
    return (strtrim);
}

