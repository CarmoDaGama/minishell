/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 08:42:00 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/02 08:42:04 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_io_out(t_branch *branch, t_global_var *global_var)
{
	int	fd;
	int	fd_term_dup;
	int	state;

	state = 0;
	fd = open(branch->io_list->value, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	fd_term_dup = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	branch->io_list = branch->io_list->next;
	if (branch->io_list)
		state = handle_redirections(branch, global_var);
	else
		call_command(branch, global_var);
	dup2(fd_term_dup, STDOUT_FILENO);
	close(fd_term_dup);
	close(fd);
	return (state);
}

int	handle_io_append(t_branch *branch, t_global_var *global_var)
{
	int	fd;
	int	fd_term_dup;
	int	state;

	state = 0;
	fd = open(branch->io_list->value, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (1);
	fd_term_dup = dup(STDOUT_FILENO);
	branch->io_list = branch->io_list->next;
	fd_term_dup = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	if (branch->io_list)
		state = handle_redirections(branch, global_var);
	else
		call_command(branch, global_var);
	dup2(fd_term_dup, STDOUT_FILENO);
	close(fd_term_dup);
	close(fd);
	return (state);
}

int	handle_io_in(t_branch *branch, t_global_var *global_var)
{
	int	fd;
	int	fd_term_dup;
	int	state;

	state = 0;
	fd = open(branch->io_list->value, O_RDONLY);
	if (fd < 0)
	{
		global_var->io_error = branch->io_list->value;
		return (127);
	}
	fd_term_dup = dup(STDIN_FILENO);
	dup2(fd, STDIN_FILENO);
	branch->io_list = branch->io_list->next;
	if (branch->io_list)
		state = handle_redirections(branch, global_var);
	else
		call_command(branch, global_var);
	dup2(fd_term_dup, STDIN_FILENO);
	close(fd_term_dup);
	close(fd);
	return (state);
}

int	handle_io_heredoc(t_branch *branch, t_global_var *global_var)
{
	int	fd;
	int	fd_term_dup;
	int	state;

	state = 0;
	fd_term_dup = dup(STDIN_FILENO);
	fd = branch->io_list->here_doc[0];
	dup2(fd, STDIN_FILENO);
	branch->io_list = branch->io_list->next;
	if (branch->io_list)
		state = handle_redirections(branch, global_var);
	else
		call_command(branch, global_var);
	dup2(fd_term_dup, STDIN_FILENO);
	close(fd_term_dup);
	close(fd);
	return (state);
}

int	handle_redirections(t_branch *branch, t_global_var *global_var)
{
	if (branch->io_list == NULL)
		return (0);
	if (branch->io_list->type == IO_OUT)
		return (handle_io_out(branch, global_var));
	if (branch->io_list->type == IO_APPEND)
		return (handle_io_append(branch, global_var));
	if (branch->io_list->type == IO_IN)
		return (handle_io_in(branch, global_var));
	if (branch->io_list->type == IO_HEREDOC)
		return (handle_io_heredoc(branch, global_var));
	return (0);
}
