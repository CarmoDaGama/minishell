/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_initializer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:40:54 by cgama             #+#    #+#             */
/*   Updated: 2024/10/16 15:40:59 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_end_branch(t_branch *branch, t_global_var *global_var)
{
	if (!branch)
		return ;
	if (branch->type == N_PIPE)
	{
		init_end_branch(branch->left, global_var);
		init_end_branch(branch->right, global_var);
	}
	else if (branch->args)
	{
		branch->expanded_args = expanding(branch->args, global_var);
	}
	if (branch->io_list && (branch->io_list->type == IO_HEREDOC))
		ft_write_heredoc(branch, global_var);
}
