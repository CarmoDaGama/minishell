/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:18:56 by cgama             #+#    #+#             */
/*   Updated: 2024/09/30 15:18:58 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_branch  *get_term(void)
{
    t_branch  *node;

    if (global_var.parse_err)
        return (NULL);
    if (ft_curr_token_is_binop() || global_var.curr_token->type == T_C_PARENT)
        return (ft_set_parse_err(1), NULL);
    else if (global_var.curr_token->type == T_O_PARENT)
    {
        ft_get_next_token();
        node = get_sequence(0);
        if (!node)
            return (ft_set_parse_err(1), NULL);
        if (!global_var.curr_token
            || global_var.curr_token->type != T_C_PARENT)
            return (ft_set_parse_err(1), node);
        ft_get_next_token();
        return (node);
    }
    else
        return (ft_get_simple_cmd());
}

t_branch  *get_sequence(int min_prec)
{
    t_branch          *left;
    t_branch          *right;
    int             n_prec;
    t_token_type    op;

    if (global_var.parse_err || !global_var.curr_token)
        return (NULL);
    left = get_term();
    if (!left)
        return (NULL);
    while (ft_curr_token_is_binop() && ft_curr_token_prec() >= min_prec)
    {
        op = global_var.curr_token->type;
        ft_get_next_token();
        if (!global_var.curr_token)
            return (ft_set_parse_err(1), left);
        n_prec = ft_prec(op) + 1;
        right = get_sequence(n_prec);
        if (!right)
            return (left);
        left = join_branch(op, left, right);
        if (!left)
            return (ft_clear_ast(&left), ft_clear_ast(&left), NULL);
    }
    return (left);
}

t_branch  *join_branch (t_token_type type_op, t_branch *left, t_branch *right)
{
    t_branch  *node;

    if (global_var.parse_err)
        return (NULL);
    node = ft_new_branch(ft_get_branch_type(type_op));
    if (!node)
        return (ft_set_parse_err(1), NULL);
    node->left = left;
    node->right = right;
    return (node);
}

t_branch *parsing(void)
{
    t_branch  *ast;

    global_var.curr_token = global_var.tokens;
    ast = get_sequence(0);
    if (global_var.curr_token)
        return (ft_set_parse_err(1), ast);
    return (ast);
}