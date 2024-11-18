/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 08:27:51 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/01 08:27:53 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit_status(t_branch *branch, t_global_var *global_var)
{
	struct stat	file_stat;

	if (!ft_strchr(branch->expanded_args[0], '/') && !global_var->io_error)
	{
		ft_putstr_fd(branch->expanded_args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_clear_ast(&global_var->ast, global_var);
	}
	else
	{
		ft_putstr_fd("bash: ", 2);
		if (global_var->io_error)
		{
			free(branch->expanded_args[0]);
			branch->expanded_args[0] = global_var->io_error;
		}
		ft_putstr_fd(branch->expanded_args[0], 2);
		if (!stat(branch->expanded_args[0], &file_stat))
			ft_putstr_fd(": is a directory\n", 2);
		else
			ft_putstr_fd(": No such file or directory\n", 2);
		ft_clear_ast(&global_var->ast, global_var);
	}
}
