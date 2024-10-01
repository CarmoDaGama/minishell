/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_chars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:29:57 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 14:29:59 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void    ft_free_char2(char **tofree)
{
    size_t  i;

    if (!tofree)
    return ;
    i = 0;
    while (tofree[i])
        free(tofree[i++]);
    free(tofree);
}

void    ft_free_char3(char ***tofree)
{
    size_t  i;

    if (!tofree)
    return ;
    i = 0;
    while (tofree[i])
        ft_free_char2(tofree[i++]);
    free(tofree);
}