/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:52:44 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 12:52:46 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_get_next_token(void)
{
    global_var.curr_token = global_var.curr_token->next;
}

bool    ft_curr_token_is_binop(void)
{
    t_token_type    type;
    if (!global_var.curr_token)
        return (false);
    type = global_var.curr_token->type;
    if (type == T_PIPE || type == T_AND || type == T_OR)
        return (true);
    return (false);
}

bool    ft_is_redir(t_token_type type)
{
    if (type == T_LESS || type == T_GREAT
        || type == T_DLESS || type == T_DGREAT)
        return (true);
    return (false);
}

int ft_prec(t_token_type type)
{
    if (type == T_AND || type == T_AND)
        return (0);
    return (1);
}

int ft_curr_token_prec()
{
    return (ft_prec(global_var.curr_token->type));
}
