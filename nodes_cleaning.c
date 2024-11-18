/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_cleaning.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:37:59 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 12:38:01 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_clear_io_list(t_io_list **lst)
{
	t_io_list	*curr_branch;
	t_io_list	*next;

	curr_branch = *lst;
	if (!curr_branch)
		return ;
	while (curr_branch)
	{
		if (curr_branch->value)
			free(curr_branch->value);
		ft_free_char2(curr_branch->expanded_value);
		next = curr_branch->next;
		if (curr_branch)
			free(curr_branch);
		curr_branch = next;
	}
	*lst = NULL;
}

void	ft_clear_cmd_branch(t_branch *node)
{
	if (!node)
		return ;
	if (node->args)
		free(node->args);
	ft_free_char2(node->expanded_args);
}

void	ft_recursive_clear_ast(t_branch *node)
{
	if (!node)
		return ;
	if (node->type == N_CMD)
		ft_clear_cmd_branch(node);
	else
	{
		if (node->left)
			ft_recursive_clear_ast(node->left);
		if (node->right)
			ft_recursive_clear_ast(node->right);
	}
	free(node);
}

void	ft_clear_ast(t_branch **ast, t_global_var *global_var)
{
	(void)global_var;
	ft_recursive_clear_ast(*ast);
	*ast = NULL;
}
