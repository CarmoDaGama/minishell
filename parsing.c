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

t_branch	*get_term(t_global_var *global_var)
{
	t_branch	*node;

	if (global_var->parse_err)
		return (NULL);
	if (ft_curr_token_is_binop(global_var)
		|| global_var->curr_token->type == T_C_PARENT)
		return (ft_set_parse_err(1, global_var), NULL);
	else if (global_var->curr_token->type == T_O_PARENT)
	{
		ft_get_next_token(global_var);
		node = get_sequence(0, global_var);
		if (!node)
			return (ft_set_parse_err(1, global_var), NULL);
		if (!global_var->curr_token
			|| global_var->curr_token->type != T_C_PARENT)
			return (ft_set_parse_err(1, global_var), node);
		ft_get_next_token(global_var);
		return (node);
	}
	else
		return (ft_get_simple_cmd(global_var));
}

t_branch	*iterator_seq(t_branch *left, t_branch *right,
	int min_prec, t_global_var *global_var)
{
	t_token_type	op;
	int				n_prec;

	while (ft_curr_token_is_binop(global_var)
		&& ft_curr_token_prec(global_var) >= min_prec)
	{
		op = global_var->curr_token->type;
		ft_get_next_token(global_var);
		if (!global_var->curr_token)
			return (ft_set_parse_err(1, global_var), left);
		n_prec = ft_prec(op) + 1;
		right = get_sequence(n_prec, global_var);
		if (!right)
			return (left);
		left = join_branch(op, left, right, global_var);
		if (!left)
			return (ft_clear_ast(&left, global_var),
				ft_clear_ast(&right, global_var), NULL);
	}
	return (left);
}

t_branch	*get_sequence(int min_prec, t_global_var *global_var)
{
	t_branch		*left;
	t_branch		*right;

	right = NULL;
	if (global_var->parse_err || !global_var->curr_token)
		return (NULL);
	left = get_term(global_var);
	if (!left)
		return (NULL);
	left = iterator_seq(left, right, min_prec, global_var);
	return (left);
}

t_branch	*join_branch(t_token_type type_op,
	t_branch *left, t_branch *right, t_global_var *global_var)
{
	t_branch	*node;

	if (global_var->parse_err)
		return (NULL);
	node = ft_new_branch(ft_get_branch_type(type_op));
	if (!node)
		return (ft_set_parse_err(1, global_var), NULL);
	node->left = left;
	node->right = right;
	return (node);
}

t_branch	*parsing(t_global_var *global_var)
{
	t_branch	*ast;

	global_var->curr_token = global_var->tokens;
	ast = get_sequence(0, global_var);
	if (global_var->curr_token)
		return (ft_set_parse_err(1, global_var), ast);
	return (ast);
}
