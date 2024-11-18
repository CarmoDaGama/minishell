/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnzita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:27:23 by dnzita            #+#    #+#             */
/*   Updated: 2024/10/04 12:27:27 by dnzita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (WIFSIGNALED(status));
}

int	run_child_pipe(t_branch *branch, int *pfds,
	t_tree_direction direction, t_global_var *global_var)
{
	if (direction == TD_LEFT)
	{
		close(pfds[0]);
		dup2(pfds[1], STDOUT_FILENO);
		close(pfds[1]);
	}
	else if (direction == TD_RIGHT)
	{
		close(pfds[1]);
		dup2(pfds[0], STDIN_FILENO);
		close(pfds[0]);
	}
	call_command(branch, global_var);
	(exit(global_var->state_exit));
	return (global_var->state_exit);
}

static int	run_pipe(t_branch *branch, t_global_var *global_var)
{
	int	status;
	int	pfds[2];
	int	pid_left;
	int	pid_right;

	pipe(pfds);
	global_var->is_child = true;
	main_signal(global_var);
	pid_left = fork();
	if (pid_left == 0)
		return (run_child_pipe(branch->left, pfds, TD_LEFT, global_var));
	pid_right = fork();
	if (pid_right == 0)
		return (run_child_pipe(branch->right, pfds, TD_RIGHT, global_var));
	(close(pfds[0]), close(pfds[1]),
		waitpid(pid_left, &status, 0), waitpid(pid_right, &status, 0));
	global_var->is_child = false;
	main_signal(global_var);
	return (get_exit_status(status));
}

void	opcions_command(t_branch *branch, t_global_var *global_var)
{
	if (!branch->expanded_args || !branch->expanded_args[0])
	{
		ft_free_char2(branch->expanded_args);
		global_var->state_exit = 0;
		return ;
	}
	if (is_builtin(branch->expanded_args[0]))
		global_var->state_exit = run_builtin(branch->expanded_args, global_var);
	else
	{
		global_var->state_exit = search_command_in_path(
				branch->expanded_args, global_var);
	}
}

void	call_command(t_branch *branch, t_global_var *global_var)
{
	if (!branch)
		global_var->state_exit = 1;
	else if (branch->type == N_PIPE)
		global_var->state_exit = run_pipe(branch, global_var);
	else
	{
		if (branch->io_list)
			global_var->state_exit = handle_redirections(branch, global_var);
		else
			opcions_command(branch, global_var);
	}
	if (global_var->state_exit == 127)
		handle_exit_status(branch, global_var);
}
