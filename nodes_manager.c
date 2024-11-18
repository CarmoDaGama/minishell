/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgama <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:14:39 by cgama             #+#    #+#             */
/*   Updated: 2024/10/01 12:14:41 by cgama            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_branch_type	ft_get_branch_type(t_token_type type)
{
	if (type == T_AND)
		return (N_AND);
	if (type == T_OR)
		return (N_OR);
	return (N_PIPE);
}

t_io_type	ft_get_io_type(t_token_type type)
{
	if (type == T_LESS)
		return (IO_IN);
	if (type == T_GREAT)
		return (IO_OUT);
	if (type == T_DLESS)
		return (IO_HEREDOC);
	return (IO_APPEND);
}

t_branch	*ft_new_branch(t_branch_type type)
{
	t_branch	*new_branch;

	new_branch = (t_branch *)ft_calloc(1, sizeof(t_branch));
	if (!new_branch)
		return (NULL);
	new_branch->type = type;
	return (new_branch);
}

t_io_list	*ft_new_io_list(t_token_type type, char *value)
{
	t_io_list	*new_io;

	new_io = manage_momery(ft_calloc(1, sizeof(t_io_list)), false);
	if (!new_io)
		return (NULL);
	new_io->type = ft_get_io_type(type);
	new_io->value = manage_momery(ft_strdup(value), false);
	if (!new_io->value)
		return (free(new_io), NULL);
	return (new_io);
}

void	ft_append_io_list(t_io_list **lst, t_io_list *new)
{
	t_io_list	*curr_branch;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	curr_branch = *lst;
	while (curr_branch && curr_branch->next)
		curr_branch = curr_branch->next;
	curr_branch->next = new;
}
